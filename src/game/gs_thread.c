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
        task++;
    }
    return 0;
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
        thread++;
    }
    return NULL;
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
void fn_800F0030(void) {
    extern u8 lbl_8047AC1C[];
    u32 r0 = 0;
    u32 r2 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r13 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047AC1C;
    r0 = *(u32*)((u8*)r3 + 0x0);
    r2 = *(u32*)((u8*)r3 + 0x8);
    r4 = *(u32*)((u8*)r3 + 0x10);
    r5 = *(u32*)((u8*)r3 + 0x14);
    r6 = *(u32*)((u8*)r3 + 0x18);
    r7 = *(u32*)((u8*)r3 + 0x1C);
    r8 = *(u32*)((u8*)r3 + 0x20);
    r9 = *(u32*)((u8*)r3 + 0x24);
    r10 = *(u32*)((u8*)r3 + 0x28);
    r11 = *(u32*)((u8*)r3 + 0x2C);
    r12 = *(u32*)((u8*)r3 + 0x30);
    r13 = *(u32*)((u8*)r3 + 0x34);
    r14 = *(u32*)((u8*)r3 + 0x38);
    r15 = *(u32*)((u8*)r3 + 0x3C);
    r16 = *(u32*)((u8*)r3 + 0x40);
    r17 = *(u32*)((u8*)r3 + 0x44);
    r18 = *(u32*)((u8*)r3 + 0x48);
    r19 = *(u32*)((u8*)r3 + 0x4C);
    r20 = *(u32*)((u8*)r3 + 0x50);
    r21 = *(u32*)((u8*)r3 + 0x54);
    r22 = *(u32*)((u8*)r3 + 0x58);
    r23 = *(u32*)((u8*)r3 + 0x5C);
    r24 = *(u32*)((u8*)r3 + 0x60);
    r25 = *(u32*)((u8*)r3 + 0x64);
    r26 = *(u32*)((u8*)r3 + 0x68);
    r27 = *(u32*)((u8*)r3 + 0x6C);
    r28 = *(u32*)((u8*)r3 + 0x70);
    r29 = *(u32*)((u8*)r3 + 0x74);
    r30 = *(u32*)((u8*)r3 + 0x78);
    r31 = *(u32*)((u8*)r3 + 0x7C);
    return;
}

/* fn_800F00C0 - 0x800F00C0 | size: 0x9C */
void fn_800F00C0(void) {
    extern u8 lbl_8047AC1C[];
    u32 r3 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;
    f32 f13 = 0.0f;
    f32 f14 = 0.0f;
    f32 f15 = 0.0f;
    f32 f16 = 0.0f;
    f32 f17 = 0.0f;
    f32 f18 = 0.0f;
    f32 f19 = 0.0f;
    f32 f20 = 0.0f;
    f32 f21 = 0.0f;
    f32 f22 = 0.0f;
    f32 f23 = 0.0f;
    f32 f24 = 0.0f;
    f32 f25 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r3 = *(u32*)lbl_8047AC1C;
    r3 = r3 + 0x88;
    f0 = *(f64*)((u8*)r3 + 0x0);
    f1 = *(f64*)((u8*)r3 + 0x8);
    f2 = *(f64*)((u8*)r3 + 0x10);
    f3 = *(f64*)((u8*)r3 + 0x18);
    f4 = *(f64*)((u8*)r3 + 0x20);
    f5 = *(f64*)((u8*)r3 + 0x28);
    f6 = *(f64*)((u8*)r3 + 0x30);
    f7 = *(f64*)((u8*)r3 + 0x38);
    f8 = *(f64*)((u8*)r3 + 0x40);
    f9 = *(f64*)((u8*)r3 + 0x48);
    f10 = *(f64*)((u8*)r3 + 0x50);
    f11 = *(f64*)((u8*)r3 + 0x58);
    f12 = *(f64*)((u8*)r3 + 0x60);
    f13 = *(f64*)((u8*)r3 + 0x68);
    f14 = *(f64*)((u8*)r3 + 0x70);
    f15 = *(f64*)((u8*)r3 + 0x78);
    f16 = *(f64*)((u8*)r3 + 0x80);
    f17 = *(f64*)((u8*)r3 + 0x88);
    f18 = *(f64*)((u8*)r3 + 0x90);
    f19 = *(f64*)((u8*)r3 + 0x98);
    f20 = *(f64*)((u8*)r3 + 0xA0);
    f21 = *(f64*)((u8*)r3 + 0xA8);
    f22 = *(f64*)((u8*)r3 + 0xB0);
    f23 = *(f64*)((u8*)r3 + 0xB8);
    f24 = *(f64*)((u8*)r3 + 0xC0);
    f25 = *(f64*)((u8*)r3 + 0xC8);
    f26 = *(f64*)((u8*)r3 + 0xD0);
    f27 = *(f64*)((u8*)r3 + 0xD8);
    f28 = *(f64*)((u8*)r3 + 0xE0);
    f29 = *(f64*)((u8*)r3 + 0xE8);
    f30 = *(f64*)((u8*)r3 + 0xF0);
    f31 = *(f64*)((u8*)r3 + 0xF8);
    return;
}

/* fn_800F028C - 0x800F028C | size: 0x68 */
void fn_800F028C(void) {
    extern u8 lbl_8047AC10[];
    extern u8 lbl_8047AC1C[];
    extern u8 lbl_8047AC20[];
    extern u8 lbl_8047AC24[];
    extern void fn_800F0030();
    extern void fn_800F00C0();
    u8 sp[0xC];
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = *(u32*)lbl_8047AC24;
    *(u32*)lbl_8047AC1C = r3;
    ((void(*)(void))fn_800F015C)();
    *(u32*)((u8*)r3 + 0x4) = r1;
    r3 = *(u32*)lbl_8047AC20;
    *(u32*)lbl_8047AC1C = r3;
    fn_800F0030();
    r5 = *(u32*)lbl_8047AC10;
    if ((u32)r5 != (u32)0x0) {
        fn_800F00C0();
    }
    r5 = *(u32*)((u8*)r3 + 0x84);
    r1 = *(u32*)((u8*)r3 + 0x4);
    r5 = 0x0;
    /* subi r5, r5, 0x1 */;
    *(u32*)((u8*)r1 + 0x0) = r5;
    r5 = *(u32*)((u8*)r3 + 0x80);
    ctr_fn = (void(*)(void))r5;
    r5 = *(u32*)((u8*)r3 + 0x14);
    r3 = *(u32*)((u8*)r3 + 0xC);
    /* indirect jump via ctr */;
}

/* fn_800F02F4 - 0x800F02F4 | size: 0x14 */
void fn_800F02F4(void) {
}

/* fn_800F0654 - 0x800F0654 | size: 0x154 */
void fn_800F0654(void) {
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;

    r29 = r3;
    r30 = r4;
    if ((s32)r0 == (s32)0) {
        *(f64*)(sp + 0x28) = f1;
        *(f64*)(sp + 0x30) = f2;
        *(f64*)(sp + 0x38) = f3;
        *(f64*)(sp + 0x40) = f4;
        *(f64*)(sp + 0x48) = f5;
        *(f64*)(sp + 0x50) = f6;
        *(f64*)(sp + 0x58) = f7;
        *(f64*)(sp + 0x60) = f8;
    }
    r0 = *(u8*)((u8*)r29 + 0x14);
    if ((u32)r0 != (u32)0x1) {
        r3 = *(u16*)((u8*)r29 + 0x20);
        fn_800E27B0();
        r31 = r3;
        r3 = *(u16*)((u8*)r29 + 0x22);
        fn_800E27B0();
        r4 = (u32)sp + 0xa8;
        r0 = (u32)sp + 0x8;
        r5 = (0x200 << 16);
        r27 = r3;
        r28 = 0xc;
        r25 = 0x8;
        *(u32*)(sp + 0x70) = r0;
        if ((s32)r30 < (s32)0x8) {
            r25 = r30;
        }
        r26 = 0x0;
        while ((s32)r26 < (s32)r25) {

            r3 = (u32)sp + 0x68;
            r4 = 0x1;
            __va_arg();
            r0 = *(u32*)((u8*)r3 + 0x0);
            r26 = r26 + 0x1;
            *(u32*)(r27 + r28) = r0;
            r28 = r28 + 0x4;

        }
        if ((s32)r30 > (s32)0x8) {
            r3 = r30 - r26;
            r0 = *(u32*)((u8*)r27 + 0x4);
            r3 = r3 << 2;
            r26 = 0x8;
            r0 = r0 - r3;
            *(u32*)((u8*)r27 + 0x4) = r0;
            r0 = *(u32*)((u8*)r27 + 0x4);
            r3 = (u32)r0 >> 2;
            r0 = r3 + 0x2;
            r28 = r0 << 2;
            while ((s32)r26 < (s32)r30) {

                r3 = (u32)sp + 0x68;
                r4 = 0x1;
                __va_arg();
                r0 = *(u32*)((u8*)r3 + 0x0);
                r26 = r26 + 0x1;
                *(u32*)(r31 + r28) = r0;
                r28 = r28 + 0x4;

            }
        }
        r3 = *(u16*)((u8*)r29 + 0x20);
        fn_800E24B0();
        r3 = *(u16*)((u8*)r29 + 0x22);
        fn_800E24B0();
    }
    return;
}

/* fn_800F0A74 - 0x800F0A74 | size: 0x4D8 */
void fn_800F0A74(void) {
    extern u8 lbl_804019F0[];
    extern u8 lbl_8047AC00[];
    extern u8 lbl_8047AC04[];
    extern u8 lbl_8047AC08[];
    extern u8 lbl_8047AC0C[];
    extern u8 lbl_8047AC10[];
    extern u8 lbl_8047AC20[];
    extern u8 lbl_8047AC24[];
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800EEA50();
    extern void fn_800EEA6C();
    extern void fn_800EEA98();
    extern void fn_800EEB34();
    extern void fn_800F028C();
    extern void fn_800F02F4();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = r4;
    r3 = r4 & 0xFF;
    r21 = *(u32*)lbl_8047AC08;
    while ((u32)r21 != (u32)0x0) {
            r0 = *(u8*)((u8*)r21 + 0x8);
            if ((u32)r0 != (u32)0x0) {
                r0 = *(u8*)((u8*)r21 + 0x9);
                if ((u32)r0 == (u32)0x0) {
                    r0 = *(u8*)((u8*)r21 + 0xA);
                    if ((u32)r0 == (u32)0x0) {
                        r0 = *(u8*)((u8*)r21 + 0x15);
                        if ((u32)r0 == (u32)0x0) {
                            if ((u32)r3 != (u32)0x0) {
                                r0 = *(u32*)((u8*)r21 + 0xC);
                                if ((u32)r0 == (u32)r30) {

                                } else {
                                    }
                                    }
                                    }
                                    }
                    r21 = *(u32*)((u8*)r21 + 0x4);
            }
            r21 = 0x0;
                                }
                                }

    if ((u32)r21 == (u32)0x0) return;
    r3 = *(u16*)((u8*)r21 + 0x22);
    fn_800E27B0();
    r0 = r3;
    r3 = *(u16*)((u8*)r21 + 0x20);
    r25 = r0;
    fn_800E27B0();
    r24 = r3;
    r4 = (u32)fn_800F02F4;
    r3 = (u32)lbl_804019F0;
    r29 = r31 & 0xFF;
    r23 = r24;
    r27 = (u32)fn_800F02F4;
    r28 = (u32)lbl_804019F0;
    while ((u32)r21 != (u32)0x0) {
            if ((u32)r21 == (u32)0x0) {
                r3 = *(u32*)lbl_8047AC08;

            } else {
                r3 = *(u32*)((u8*)r21 + 0x4);
                goto L_800F0B94;
                do {
                    r0 = *(u8*)((u8*)r3 + 0x8);
                    if ((u32)r0 == (u32)0x0) continue;
                    r0 = *(u8*)((u8*)r3 + 0x9);
                    if ((u32)r0 != (u32)0x0) continue;
                    r0 = *(u8*)((u8*)r3 + 0xA);
                    if ((u32)r0 != (u32)0x0) continue;
                    r0 = *(u8*)((u8*)r3 + 0x15);
                    if ((u32)r0 != (u32)0x0) continue;
                    if ((u32)r29 == (u32)0x0) goto L_800F0BA0;
                    r0 = *(u32*)((u8*)r3 + 0xC);
                    if ((u32)r0 != (u32)r30) continue;
                    goto L_800F0BA0;

                    r3 = *(u32*)((u8*)r3 + 0x4);
                L_800F0B94: ;
            } while ((u32)r3 != (u32)0x0);
            r3 = 0x0;
            L_800F0BA0: ;
            *(u32*)lbl_8047AC04 = r3;
            if ((u32)r3 != (u32)0x0) {
                r3 = *(u16*)((u8*)r3 + 0x20);
                fn_800E27B0();
                r22 = r3;
            }
            *(u32*)lbl_8047AC00 = r21;
            r0 = 0x1;
            *(u8*)((u8*)r21 + 0x14) = r0;
            r0 = *(u32*)((u8*)r25 + 0x4);
            r0 = r23 + r0;
            *(u32*)((u8*)r25 + 0x4) = r0;
            *(u32*)((u8*)r28 + 0x84) = r27;
            *(u32*)lbl_8047AC24 = r28;
            *(u32*)lbl_8047AC20 = r25;
            r0 = *(u8*)((u8*)r21 + 0x16);
            *(u32*)lbl_8047AC10 = r0;
            fn_800F028C();
            r3 = *(u32*)((u8*)r25 + 0x4);
            r0 = 0x0;
            r3 = r3 - r23;
            *(u32*)((u8*)r25 + 0x4) = r3;
            *(u32*)lbl_8047AC00 = r0;
            r0 = *(u8*)((u8*)r21 + 0x15);
            if ((u32)r0 != (u32)0x0) {
                r3 = r23;
                fn_800EEA50();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x0) {
                    r3 = r23;
                    fn_800EEB34();
                }
                r3 = *(u16*)((u8*)r21 + 0x22);
                fn_800E24B0();
                r3 = *(u16*)((u8*)r21 + 0x20);
                fn_800E24B0();
                r0 = *(u32*)lbl_8047AC00;
                if ((u32)r21 != (u32)r0) {
                    r0 = *(u32*)lbl_8047AC04;
                    if ((u32)r21 == (u32)r0) {
                    }
                    r3 = 0x1;
                    *(u8*)((u8*)r21 + 0x15) = r3;
                    r0 = *(u32*)lbl_8047AC04;
                    if ((u32)r21 == (u32)r0) {
                        *(u8*)lbl_8047AC0C = r3;

                        } else {
                        r0 = 0x0;
                        *(u8*)((u8*)r21 + 0x14) = r0;
                        *(u8*)((u8*)r21 + 0x8) = r0;
                        r3 = *(u32*)((u8*)r21 + 0x0);
                        if ((u32)r3 != (u32)0x0) {
                            r0 = *(u32*)((u8*)r21 + 0x4);
                            *(u32*)((u8*)r3 + 0x4) = r0;
                        }
                        r3 = *(u32*)((u8*)r21 + 0x4);
                        if ((u32)r3 != (u32)0x0) {
                            r0 = *(u32*)((u8*)r21 + 0x0);
                            *(u32*)((u8*)r3 + 0x0) = r0;
                        }
                        r0 = *(u32*)lbl_8047AC08;
                        if ((u32)r0 == (u32)r21) {
                            r0 = *(u32*)((u8*)r21 + 0x4);
                            *(u32*)lbl_8047AC08 = r0;
                        }
                        r3 = *(u16*)((u8*)r21 + 0x20);
                        fn_800E209C();
                        r3 = *(u16*)((u8*)r21 + 0x22);
                        fn_800E209C();

                    }
                } else {
                        r3 = r23;
                        fn_800EEA50();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 != (u32)r21) {
                            r4 = *(u32*)((u8*)r21 + 0x10);
                            r3 = r24;
                            DCFlushRange();
                            r5 = *(u32*)((u8*)r21 + 0x10);
                            r3 = r24;
                            r4 = r23;
                            fn_800EEA98();
                            fn_800EEA6C();
                            r3 = r23;
                            fn_800EEB34();
                        }
                        r3 = *(u16*)((u8*)r21 + 0x22);
                        fn_800E24B0();
                        r3 = *(u16*)((u8*)r21 + 0x20);
                        fn_800E24B0();
                }
                }
            r0 = *(u8*)lbl_8047AC0C;
            if ((u32)r0 != (u32)0x0) {
                if ((u32)r21 == (u32)0x0) {
                    r4 = *(u32*)lbl_8047AC08;
                } else {

                    r4 = *(u32*)((u8*)r21 + 0x4);
                }
                r3 = r31 & 0xFF;
                while ((u32)r4 != (u32)0x0) {
                        r0 = *(u8*)((u8*)r4 + 0x8);
                        if ((u32)r0 != (u32)0x0) {
                            r0 = *(u8*)((u8*)r4 + 0x9);
                            if ((u32)r0 == (u32)0x0) {
                                r0 = *(u8*)((u8*)r4 + 0xA);
                                if ((u32)r0 == (u32)0x0) {
                                    r0 = *(u8*)((u8*)r4 + 0x15);
                                    if ((u32)r0 == (u32)0x0) {
                                        if ((u32)r3 != (u32)0x0) {
                                            r0 = *(u32*)((u8*)r4 + 0xC);
                                            if ((u32)r0 == (u32)r30) {

                                            } else {
                                                }
                                                }
                                                }
                                                }
                                r4 = *(u32*)((u8*)r4 + 0x4);
                        }
                        r4 = 0x0;
                                            }
                                            }

                r0 = *(u32*)lbl_8047AC04;
                if ((u32)r0 == (u32)r4) {
                }
                r3 = *(u32*)lbl_8047AC04;
                r21 = r3;
                if ((u32)r3 != (u32)0x0) {
                    r3 = *(u16*)((u8*)r3 + 0x22);
                    r23 = r22;
                    r24 = r22;
                    fn_800E27B0();
                    r25 = r3;

                    } else {
                    r3 = r22;
                    fn_800EEA50();
                    r0 = r3 & 0xFF;
                    if ((u32)r3 != (u32)0x0) {
                        fn_800EEA6C();
                        r3 = r22;
                        fn_800EEB34();
                    }
                    r3 = *(u32*)lbl_8047AC04;
                    if ((u32)r3 != (u32)0x0) {
                        r3 = *(u16*)((u8*)r3 + 0x20);
                        fn_800E24B0();
                        r26 = *(u32*)lbl_8047AC04;
                        r0 = *(u8*)((u8*)r26 + 0x15);
                        if ((u32)r0 != (u32)0x0) {
                            r0 = *(u32*)lbl_8047AC00;
                            r3 = 0x0;
                            *(u32*)lbl_8047AC04 = r3;

                            if ((u32)r26 == (u32)r0 && (u32)r26 != (u32)0x0) {

                                r3 = 0x1;
                                *(u8*)((u8*)r26 + 0x15) = r3;
                                r0 = *(u32*)lbl_8047AC04;
                                if ((u32)r26 == (u32)r0) {
                                    *(u8*)lbl_8047AC0C = r3;

                                } else {
                                    *(u8*)((u8*)r26 + 0x14) = r3;
                                    *(u8*)((u8*)r26 + 0x8) = r3;
                                    r3 = *(u32*)((u8*)r26 + 0x0);
                                    if ((u32)r3 != (u32)0x0) {
                                        r0 = *(u32*)((u8*)r26 + 0x4);
                                        *(u32*)((u8*)r3 + 0x4) = r0;
                                    }
                                    r3 = *(u32*)((u8*)r26 + 0x4);
                                    if ((u32)r3 != (u32)0x0) {
                                        r0 = *(u32*)((u8*)r26 + 0x0);
                                        *(u32*)((u8*)r3 + 0x0) = r0;
                                    }
                                    r0 = *(u32*)lbl_8047AC08;
                                    if ((u32)r0 == (u32)r26) {
                                        r0 = *(u32*)((u8*)r26 + 0x4);
                                        *(u32*)lbl_8047AC08 = r0;
                                    }
                                    r3 = *(u16*)((u8*)r26 + 0x20);
                                    fn_800E209C();
                                    r3 = *(u16*)((u8*)r26 + 0x22);
                                    fn_800E209C();
                                }
                            }
                            }
                                }
                    if ((u32)r21 == (u32)0x0) {
                        r21 = *(u32*)lbl_8047AC08;
                    } else {

                        r21 = *(u32*)((u8*)r21 + 0x4);
                    }
                    r3 = r31 & 0xFF;
                    while ((u32)r21 != (u32)0x0) {
                            r0 = *(u8*)((u8*)r21 + 0x8);
                            if ((u32)r0 != (u32)0x0) {
                                r0 = *(u8*)((u8*)r21 + 0x9);
                                if ((u32)r0 == (u32)0x0) {
                                    r0 = *(u8*)((u8*)r21 + 0xA);
                                    if ((u32)r0 == (u32)0x0) {
                                        r0 = *(u8*)((u8*)r21 + 0x15);
                                        if ((u32)r0 == (u32)0x0) {
                                            if ((u32)r3 != (u32)0x0) {
                                                r0 = *(u32*)((u8*)r21 + 0xC);
                                                if ((u32)r0 == (u32)r30) {

                                                } else {
                                                    }
                                                    }
                                                    }
                                                    }
                                    r21 = *(u32*)((u8*)r21 + 0x4);
                            }
                            r21 = 0x0;
                                                }
                                                }

                    *(u32*)lbl_8047AC04 = r21;
                    if ((u32)r21 != (u32)0x0) {
                        r3 = *(u16*)((u8*)r21 + 0x22);
                        fn_800E27B0();
                        r0 = r3;
                        r3 = *(u16*)((u8*)r21 + 0x20);
                        r25 = r0;
                        fn_800E27B0();
                        r24 = r3;
                        r23 = r24;
                        }
                    }
                    }
            r0 = 0x0;
            *(u8*)lbl_8047AC0C = r0;
    }

    return;
}
}

/* fn_800F106C - 0x800F106C | size: 0x7C */
void fn_800F106C(void) {
    extern u8 lbl_8047AC38[];
    extern u8 lbl_8047AC3C[];
    extern u8 lbl_8047AC40[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    *(u32*)(sp + 0x8) = r0;
    r4 = *(u32*)lbl_8047AC38;
    ctr_fn = (void(*)(void))r4;
    r3 = *(u32*)lbl_8047AC3C;
    f8 = *(f32*)((u8*)r3 + 0x1C);
    f7 = *(f32*)((u8*)r3 + 0x18);
    f6 = *(f32*)((u8*)r3 + 0x14);
    f5 = *(f32*)((u8*)r3 + 0x10);
    f4 = *(f32*)((u8*)r3 + 0xC);
    f3 = *(f32*)((u8*)r3 + 0x8);
    f2 = *(f32*)((u8*)r3 + 0x4);
    f1 = *(f32*)((u8*)r3 + 0x0);
    r3 = *(u32*)lbl_8047AC40;
    r10 = *(u32*)((u8*)r3 + 0x1C);
    r9 = *(u32*)((u8*)r3 + 0x18);
    r8 = *(u32*)((u8*)r3 + 0x14);
    r7 = *(u32*)((u8*)r3 + 0x10);
    r6 = *(u32*)((u8*)r3 + 0xC);
    r5 = *(u32*)((u8*)r3 + 0x8);
    r4 = *(u32*)((u8*)r3 + 0x4);
    r3 = *(u32*)((u8*)r3 + 0x0);
    /* crclr cr1eq */;
    ctr_fn();
    return;
}

/* fn_800F10E8 - 0x800F10E8 | size: 0x2E8 */
void fn_800F10E8(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8027107C[];
    extern u8 lbl_80401BB8[];
    extern u8 lbl_80401BD8[];
    extern u8 lbl_80478B00[];
    extern u8 lbl_8047AC38[];
    extern u8 lbl_8047AC3C[];
    extern u8 lbl_8047AC40[];
    extern void fn_800DD38C();
    extern void fn_800F106C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = 0x0;
    r29 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r29 + 0x14) = r0;
    r3 = *(u32*)((u8*)r29 + 0x14);
    r31 = *(u16*)((u8*)r3 + 0x0);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r29 + 0x14) = r0;
    r4 = *(u32*)((u8*)r29 + 0x28);
    r5 = *(u32*)((u8*)r29 + 0x1C);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r5;
    }
    r4 = *(u32*)((u8*)r29 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r31;
    }
    r0 = *(u32*)((u8*)r29 + 0x28);
    r3 = r31 + 0x2;
    r0 = r0 - r3;
    *(u32*)((u8*)r29 + 0x1C) = r0;
    r0 = *(u32*)((u8*)r29 + 0x1C);
    r4 = *(u32*)lbl_80478B00;
    r3 = r0 << 2;
    r0 = r3 + 0x6c;
    r3 = *(u32*)((u8*)r4 + 0x10);
    r0 = *(u32*)(r29 + r0);
    r0 = r0 * 0xc;
    r31 = r3 + r0;
    r0 = *(u32*)((u8*)r31 + 0x0);
    *(u32*)lbl_8047AC38 = r0;
    if ((u32)r0 != (u32)0x0) {
        r3 = (u32)lbl_80401BD8;
        r30 = 0x0;
        r3 = (u32)lbl_80401BD8;
        r28 = 0x0;
        r4 = 0x0;
        r5 = 0x20;
        memset((void*)r3, (int)r4, (u32)r5);
        r3 = (u32)lbl_80401BB8;
        r4 = 0x0;
        r3 = (u32)lbl_80401BB8;
        r5 = 0x20;
        memset((void*)r3, (int)r4, (u32)r5);
        r3 = *(u32*)((u8*)r29 + 0x1C);
        r0 = *(u32*)((u8*)r29 + 0x28);
        r3 = r0 - r3;
        /* subi r0, r3, 0x3 */;
        if ((s32)r0 > (s32)0x8) {
            r0 = 0x8;
        }
        r5 = 0x0;
        ctr_fn = (void(*)(void))r0;
        if ((s32)r0 > (s32)0x0) {
            do {
                r0 = r5 + 0x4;
                r0 = *(u8*)(r31 + r0);
                if ((u32)r0 == (u32)0x0) break;
                if ((u32)r0 == (u32)0x2) {
                    r0 = *(u32*)((u8*)r29 + 0x1C);
                    r3 = (u32)lbl_80401BB8;
                    r3 = (u32)lbl_80401BB8;
                    r4 = r5 + r0;
                    r0 = r4 + 0x1;
                    r4 = r0 << 2;
                    r0 = r4 + 0x6c;
                    f0 = *(f32*)(r29 + r0);
                    *(f32*)(r3 + r30) = f0;
                    r30 = r30 + 0x4;
                } else {

                    r0 = *(u32*)((u8*)r29 + 0x1C);
                    r3 = (u32)lbl_80401BD8;
                    r3 = (u32)lbl_80401BD8;
                    r4 = r5 + r0;
                    r0 = r4 + 0x1;
                    r4 = r0 << 2;
                    r0 = r4 + 0x6c;
                    r0 = *(u32*)(r29 + r0);
                    *(u32*)(r3 + r28) = r0;
                    r28 = r28 + 0x4;
                }
                r5 = r5 + 0x1;
            } while (--ctr != 0);
        }
        r4 = (u32)lbl_80401BB8;
        r3 = (u32)lbl_80401BD8;
        r4 = (u32)lbl_80401BB8;
        r0 = (u32)lbl_80401BD8;
        *(u32*)lbl_8047AC3C = r4;
        *(u32*)lbl_8047AC40 = r0;
        fn_800F106C();
        r30 = r3;
    }
    r0 = *(u32*)((u8*)r29 + 0x1C);
    r0 = r0 << 2;
    r3 = r29 + r0;
    *(u32*)((u8*)r3 + 0x68) = r30;
    r3 = *(u32*)((u8*)r29 + 0x28);
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r29 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    }
    r3 = *(u32*)((u8*)r29 + 0x28);
    *(u32*)(sp + 0x10) = r0;
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r29 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    }
    r3 = (u32)lbl_8027107C;
    r30 = (u32)lbl_8027107C;
    *(u32*)(sp + 0x14) = r0;
    r31 = 0x0;
    *(u32*)((u8*)r29 + 0x1C) = r0;
    while ((s32)r31 < (s32)r28) {

        r3 = *(u32*)((u8*)r29 + 0x28);
        if ((s32)r3 <= (s32)0x0) {
            r3 = r30;
            /* crclr cr1eq */;
            fn_800DD38C();
        } else {

            /* subi r0, r3, 0x1 */;
            *(u32*)((u8*)r29 + 0x28) = r0;
        }
        r31 = r31 + 0x1;

    }
    r3 = 0x1;
    return;
}

/* fn_800F13D0 - 0x800F13D0 | size: 0x2F0 */
void fn_800F13D0(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8027107C[];
    extern void fn_800D3088();
    extern void fn_800DD38C();
    extern void fn_800F0308();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r31 + 0x14) = r0;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r29 = *(u16*)((u8*)r3 + 0x0);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r31 + 0x14) = r0;
    r4 = *(u32*)((u8*)r31 + 0x28);
    r5 = *(u32*)((u8*)r31 + 0x1C);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r5;
    }
    r4 = *(u32*)((u8*)r31 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r29;
    }
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = r29 + 0x2;
    r0 = 0x2;
    r30 = 0x0;
    r3 = r3 - r4;
    r29 = 0x3;
    *(u32*)((u8*)r31 + 0x1C) = r3;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    r3 = r3 << 2;
    r3 = r3 + 0x6c;
    r28 = *(u32*)(r31 + r3);
    *(u8*)((u8*)r31 + 0x4) = r0;
    while ((s32)r30 < (s32)r28) {

        r0 = *(u8*)((u8*)r31 + 0x4);
        if ((u32)r0 == (u32)0x4) {
            *(u8*)((u8*)r31 + 0x4) = r29;
        }
        r0 = *(u8*)((u8*)r31 + 0x4);
        if ((u32)r0 == (u32)0x3) {
            r3 = *(u32*)((u8*)r31 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = (u32)lbl_8027107C;
                r3 = (u32)lbl_8027107C;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r31 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r31 + 0x28) = r3;
                r3 = r31 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            r3 = *(u32*)((u8*)r31 + 0x28);
            *(u32*)(sp + 0x20) = r0;
            if ((s32)r3 <= (s32)0x0) {
                r3 = (u32)lbl_8027107C;
                r3 = (u32)lbl_8027107C;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r31 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r31 + 0x28) = r3;
                r3 = r31 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r3 = (u32)lbl_8027107C;
            r30 = (u32)lbl_8027107C;
            *(u32*)(sp + 0x24) = r0;
            r28 = 0x0;
            *(u32*)((u8*)r31 + 0x1C) = r0;
            while ((s32)r28 < (s32)r29) {

                r3 = *(u32*)((u8*)r31 + 0x28);
                if ((s32)r3 <= (s32)0x0) {
                    r3 = r30;
                    /* crclr cr1eq */;
                    fn_800DD38C();
                } else {

                    /* subi r0, r3, 0x1 */;
                    *(u32*)((u8*)r31 + 0x28) = r0;
                }
                r28 = r28 + 0x1;

            }
            r3 = 0x0;
            return;
        }
        fn_800F0308();
        fn_800D3088();
        r30 = r30 + r3;

    }
    r0 = 0x1;
    *(u8*)((u8*)r31 + 0x4) = r0;
    r3 = *(u32*)((u8*)r31 + 0x28);
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    }
    r3 = *(u32*)((u8*)r31 + 0x28);
    *(u32*)(sp + 0x18) = r0;
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    }
    r3 = (u32)lbl_8027107C;
    r29 = (u32)lbl_8027107C;
    *(u32*)(sp + 0x1C) = r0;
    r28 = 0x0;
    *(u32*)((u8*)r31 + 0x1C) = r0;
    while ((s32)r28 < (s32)r30) {

        r3 = *(u32*)((u8*)r31 + 0x28);
        if ((s32)r3 <= (s32)0x0) {
            r3 = r29;
            /* crclr cr1eq */;
            fn_800DD38C();
        } else {

            /* subi r0, r3, 0x1 */;
            *(u32*)((u8*)r31 + 0x28) = r0;
        }
        r28 = r28 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* fn_800F16C0 - 0x800F16C0 | size: 0x34C */
void fn_800F16C0(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8027107C[];
    extern u8 lbl_80401A78[];
    extern u8 lbl_80401AB8[];
    extern u8 lbl_8047CCB8[];
    extern void fn_800C8520();
    extern void fn_800DD38C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r31 + 0x14) = r0;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r30 = *(u16*)((u8*)r3 + 0x0);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r31 + 0x14) = r0;
    r4 = *(u32*)((u8*)r31 + 0x28);
    r5 = *(u32*)((u8*)r31 + 0x1C);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r5;
    }
    r4 = *(u32*)((u8*)r31 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r30;
    }
    r0 = *(u32*)((u8*)r31 + 0x28);
    r4 = r30 + 0x2;
    r3 = (u32)lbl_80401AB8;
    r28 = 0x1;
    r0 = r0 - r4;
    *(u32*)((u8*)r31 + 0x1C) = r0;
    r3 = (u32)lbl_80401AB8;
    r26 = r3;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    r30 = r3 + 0xff;
    r3 = r0 << 2;
    r0 = r3 + 0x6c;
    r0 = *(u32*)(r31 + r0);
    *(u32*)(sp + 0x1C) = r0;
    r27 = r0;
    while ((u32)r26 != (u32)r30) {
            r3 = *(u8*)((u8*)r27 + 0x0);
            r0 = (s8)r3;
            r0 = (s8)r3;
            if ((s32)r0 == (s32)0x25) {
                r0 = *(u32*)((u8*)r31 + 0x1C);
                r3 = (u32)lbl_80401A78;
                r4 = (u32)lbl_80401A78;
                r0 = r0 + r28;
                r3 = 0x0;
                r5 = r0 << 2;
                r28 = r28 + 0x1;
                r0 = r5 + 0x6c;
                r0 = *(u32*)(r31 + r0);
                *(u32*)(sp + 0x8) = r0;
                while (1) {
                    r0 = *(u8*)((u8*)r27 + 0x0);
                    r3 = r3 + 0x1;
                    *(u8*)((u8*)r4 + 0x0) = r0;
                    r4 = r4 + 0x1;
                    r5 = *(u8*)((u8*)r27 + 0x0);
                    r0 = (s8)r5;
                    if ((s32)r0 != (s32)0x64 || (s32)r0 == (s32)0x78) {

                        if ((s32)r0 == (s32)0x63) {
                        }
                        r4 = (u32)lbl_80401A78;
                        r0 = 0x0;
                        r4 = (u32)lbl_80401A78;
                        *(u8*)(r4 + r3) = r0;
                        r3 = r26;
                        /* crclr cr1eq */;
                        fn_800C8520();
                        r29 = r3;
                        break;
                        }
                    if ((s32)r0 == (s32)0x66) {
                        r4 = (u32)lbl_80401A78;
                        r0 = 0x0;
                        r4 = (u32)lbl_80401A78;
                        f1 = *(f32*)(sp + 0x8);
                        *(u8*)(r4 + r3) = r0;
                        r3 = r26;
                        /* crset cr1eq */;
                        fn_800C8520();
                        r29 = r3;

                    } else if ((s32)r0 == (s32)0x73) {
                        r4 = (u32)lbl_80401A78;
                        r0 = 0x0;
                        r4 = (u32)lbl_80401A78;
                        *(u8*)(r4 + r3) = r0;
                        r3 = r26;
                        /* crclr cr1eq */;
                        fn_800C8520();
                        r29 = r3;

                    } else {
                        r0 = (s8)r5;
                        if ((s32)r0 == (s32)0x73) break;
                        r27 = r27 + 0x1;
                }
                }

                r26 = r26 + r29;

            } else {
                if ((s32)r0 == (s32)0x5c) {
                    r0 = *(u8*)((u8*)r27 + 0x1);
                    if ((s32)r0 != (s32)0x6e) {
                    *(u8*)((u8*)r26 + 0x0) = r3;
                    r26 = r26 + 0x1;

                    } else {
                    }
                    *(u8*)((u8*)r26 + 0x0) = r3;
                    r26 = r26 + 0x1;
                }
                    }

            r27 = r27 + 0x1;
            if ((u32)r26 >= (u32)r30) break;
    }

    r0 = 0x0;
    r3 = (u32)lbl_80401AB8;
    *(u8*)((u8*)r26 + 0x0) = r0;
    r3 = (u32)lbl_80401AB8;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = *(u32*)((u8*)r31 + 0x28);
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0x10) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x10) = r0;
    }
    r3 = *(u32*)((u8*)r31 + 0x28);
    *(u32*)(sp + 0x14) = r0;
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    }
    r3 = (u32)lbl_8027107C;
    r27 = (u32)lbl_8027107C;
    *(u32*)(sp + 0x18) = r0;
    r28 = 0x0;
    *(u32*)((u8*)r31 + 0x1C) = r0;
    while ((s32)r28 < (s32)r26) {

        r3 = *(u32*)((u8*)r31 + 0x28);
        if ((s32)r3 <= (s32)0x0) {
            r3 = r27;
            /* crclr cr1eq */;
            fn_800DD38C();
        } else {

            /* subi r0, r3, 0x1 */;
            *(u32*)((u8*)r31 + 0x28) = r0;
        }
        r28 = r28 + 0x1;

    }
    r3 = 0x1;
    return;
}

/* fn_800F1A0C - 0x800F1A0C | size: 0x42C */
void fn_800F1A0C(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCBC[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r3 = 0x0;
            if ((s32)r3 == (s32)0x0) {
                if ((s32)r0 != (s32)0x0) {
                }
                r3 = 0x1;
                }
            r0 = -r3;
            r0 = r0 | r3;
            r0 = (u32)r0 >> 31;
            *(u32*)(sp + 0x38) = r0;

        } else {
            r3 = 0x0;
            if ((s32)r3 == (s32)0x0) {
                f1 = *(f32*)(sp + 0x3C);
                f0 = *(f32*)lbl_8047CCBC;
                if (f1 != f0) {
                }
                r3 = 0x1;
                }
            r0 = -r3;
            r0 = r0 | r3;
            r0 = (u32)r0 >> 31;
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                f1 = *(f32*)(sp + 0x40);
                r3 = 0x0;
                f0 = *(f32*)lbl_8047CCBC;
                if (f1 == f0) {
                    if ((s32)r0 != (s32)0x0) {
                    }
                    r3 = 0x1;
                    }
                r0 = -r3;
                r0 = r0 | r3;
                r0 = (u32)r0 >> 31;
                *(u32*)(sp + 0x38) = r0;

            } else {
                f0 = *(f32*)(sp + 0x40);
                r3 = 0x0;
                f1 = *(f32*)lbl_8047CCBC;
                if (f0 == f1) {
                    f0 = *(f32*)(sp + 0x3C);
                    if (f0 != f1) {
                    }
                    r3 = 0x1;
                    }
                r0 = -r3;
                r0 = r0 | r3;
                r0 = (u32)r0 >> 31;
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F1E38 - 0x800F1E38 | size: 0x42C */
void fn_800F1E38(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCBC[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r3 = 0x0;
            if (((s32)r3 != (s32)0x0) && ((s32)r0 != (s32)0x0)) {

                r3 = 0x1;
            }
            r0 = -r3;
            r0 = r0 | r3;
            r0 = (u32)r0 >> 31;
            *(u32*)(sp + 0x38) = r0;

        } else {
            r3 = 0x0;
            if ((s32)r3 != (s32)0x0) {
                f1 = *(f32*)(sp + 0x3C);
                f0 = *(f32*)lbl_8047CCBC;
                if (f1 != f0) {
                    r3 = 0x1;
            }
            }
            r0 = -r3;
            r0 = r0 | r3;
            r0 = (u32)r0 >> 31;
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                f1 = *(f32*)(sp + 0x40);
                r3 = 0x0;
                f0 = *(f32*)lbl_8047CCBC;
                if ((f1 != f0) && ((s32)r0 != (s32)0x0)) {

                    r3 = 0x1;
                }
                r0 = -r3;
                r0 = r0 | r3;
                r0 = (u32)r0 >> 31;
                *(u32*)(sp + 0x38) = r0;

            } else {
                f0 = *(f32*)(sp + 0x40);
                r3 = 0x0;
                f1 = *(f32*)lbl_8047CCBC;
                if (f0 != f1) {
                    f0 = *(f32*)(sp + 0x3C);
                    if (f0 != f1) {
                        r3 = 0x1;
                }
                }
                r0 = -r3;
                r0 = r0 | r3;
                r0 = (u32)r0 >> 31;
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F2264 - 0x800F2264 | size: 0x290 */
void fn_800F2264(void) {
    extern u8 lbl_80271068[];
    extern void fn_800DD38C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)lbl_80271068;
    r27 = r3;
    r31 = (u32)lbl_80271068;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r3 + 0x0);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r30 = *(u8*)((u8*)r4 + 0x0);
    r29 = *(u16*)((u8*)r3 + 0x0);
    r3 = r3 + 0x2;
    r0 = r30 & 0x00000080;
    *(u32*)((u8*)r27 + 0x14) = r3;
    if ((s32)r0 != (s32)0) {
        r3 = r31 + 0x48;
        /* crclr cr1eq */;
        fn_800DD38C();
        r3 = 0x1;
        return;
    }
    r0 = r28 & 0x00000080;
    if ((s32)r0 != (s32)0) {
        r0 = *(u32*)((u8*)r27 + 0x28);
        r0 = r0 - r29;
        *(u32*)((u8*)r27 + 0x28) = r0;
        r0 = *(u32*)((u8*)r27 + 0x28);
        r3 = r0 << 2;
        r28 = r3 + 0x6c;
        r28 = r27 + r28;

    } else {
        r3 = *(u32*)((u8*)r27 + 0x28);
        if ((s32)r3 <= (s32)0x0) {
            r3 = r31 + 0x14;
            /* crclr cr1eq */;
            fn_800DD38C();
            r0 = *(u32*)((u8*)r27 + 0x6C);
            *(u32*)(sp + 0xC) = r0;
        } else {

            /* subi r3, r3, 0x1 */;
            r0 = r3 << 2;
            *(u32*)((u8*)r27 + 0x28) = r3;
            r3 = r27 + r0;
            r0 = *(u32*)((u8*)r3 + 0x6C);
            *(u32*)(sp + 0xC) = r0;
        }
        r0 = r28 & 0x00000040;
        if ((s32)r3 != (s32)0x0) {
            r0 = *(u32*)((u8*)r27 + 0x1C);
            r0 = r0 + r4;
            r3 = r0 << 2;
            r28 = r3 + 0x6c;
            r28 = r27 + r28;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x18);
            r0 = r4 << 2;
            r28 = r3 + r0;
        }
        }

    r3 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r3 <= (s32)0x0) {
        r3 = r31 + 0x14;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r27 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    }
    r0 = r30 & 0x00000040;
    if ((s32)r3 != (s32)0x0) {
        r0 = *(u32*)((u8*)r27 + 0x1C);
        r0 = r0 + r4;
        r3 = r0 << 2;
        r6 = r3 + 0x6c;
        r6 = r27 + r6;
    } else {

        r3 = *(u32*)((u8*)r27 + 0x18);
        r0 = r4 << 2;
        r6 = r3 + r0;
    }
    r7 = 0x0;
    if ((s32)r29 > (s32)0x0) {
        /* subi r3, r29, 0x8 */;
        if ((s32)r29 > (s32)0x8) {
            r0 = r3 + 0x7;
            r4 = r28;
            r0 = (u32)r0 >> 3;
            r5 = r6;
            ctr_fn = (void(*)(void))r0;
            if ((s32)r3 > (s32)0x0) {
                do {
                    r0 = *(u32*)((u8*)r4 + 0x0);
                    r7 = r7 + 0x8;
                    *(u32*)((u8*)r5 + 0x0) = r0;
                    r0 = *(u32*)((u8*)r4 + 0x4);
                    *(u32*)((u8*)r5 + 0x4) = r0;
                    r0 = *(u32*)((u8*)r4 + 0x8);
                    *(u32*)((u8*)r5 + 0x8) = r0;
                    r0 = *(u32*)((u8*)r4 + 0xC);
                    *(u32*)((u8*)r5 + 0xC) = r0;
                    r0 = *(u32*)((u8*)r4 + 0x10);
                    *(u32*)((u8*)r5 + 0x10) = r0;
                    r0 = *(u32*)((u8*)r4 + 0x14);
                    *(u32*)((u8*)r5 + 0x14) = r0;
                    r0 = *(u32*)((u8*)r4 + 0x18);
                    *(u32*)((u8*)r5 + 0x18) = r0;
                    r0 = *(u32*)((u8*)r4 + 0x1C);
                    r4 = r4 + 0x20;
                    *(u32*)((u8*)r5 + 0x1C) = r0;
                    r5 = r5 + 0x20;
                } while (--ctr != 0);
        }
        }
        r4 = r7 << 2;
        r0 = r29 - r7;
        r3 = r28 + r4;
        r4 = r6 + r4;
        ctr_fn = (void(*)(void))r0;
        if ((s32)r7 < (s32)r29) {
            do {
                r0 = *(u32*)((u8*)r3 + 0x0);
                r3 = r3 + 0x4;
                *(u32*)((u8*)r4 + 0x0) = r0;
                r4 = r4 + 0x4;
            } while (--ctr != 0);
    }
    }
    r28 = r6;
    r30 = 0x0;
    while ((s32)r30 < (s32)r29) {

        r3 = *(u32*)((u8*)r27 + 0x28);
        r4 = *(u32*)((u8*)r28 + 0x0);
        if ((s32)r3 > (s32)0x40) {
            r3 = r31 + 0x0;
            /* crclr cr1eq */;
            fn_800DD38C();
        } else {

            r0 = r3 + 0x1;
            r3 = r3 << 2;
            *(u32*)((u8*)r27 + 0x28) = r0;
            r0 = r3 + 0x6c;
            *(u32*)(r27 + r0) = r4;
        }
        r28 = r28 + 0x4;
        r30 = r30 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* fn_800F24F4 - 0x800F24F4 | size: 0x2E0 */
void fn_800F24F4(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x1C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x24) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x24) = r0;
    }

    r0 = r29 & 0x3F;
    r4 = r28 & 0x3F;
    if ((s32)r0 != (s32)r4) {
        if ((s32)r4 == (s32)0x2) {
            /* xoris r3, r3, 0x8000 */;
            r0 = (0x4330 << 16);
            f1 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x30) = r0;
            f0 = *(f64*)(sp + 0x30);
            f0 = f0 - f1;
            *(f32*)(sp + 0x28) = f0;

        } else {
            f0 = *(f32*)(sp + 0x28);
            f0 = (f64)(s32)f0;
            *(f64*)(sp + 0x30) = f0;
            *(u32*)(sp + 0x28) = r0;
        }
        }

    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x84;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();

    } else {
        r0 = r29 & 0x00000080;
        r28 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = r31 + 0xa0;
            /* crclr cr1eq */;
            fn_800DD38C();

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            }
            r0 = r28 & 0x00000040;
            if ((s32)r3 != (s32)0x0) {
                r3 = *(u32*)((u8*)r27 + 0x1C);
                r3 = r3 + r5;
                r3 = r3 << 2;
                r3 = r27 + r3;
                *(u32*)((u8*)r3 + 0x6C) = r0;

            } else {
                r4 = *(u32*)((u8*)r27 + 0x18);
                r3 = r5 << 2;
                *(u32*)(r4 + r3) = r0;
            }
            }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F27D4 - 0x800F27D4 | size: 0x414 */
void fn_800F27D4(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r3 = r0 - r4;
            r0 = r4 - r0;
            r0 = r3 | r0;
            r0 = (u32)r0 >> 31;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r4, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            if (f1 != f0) {
                r0 = 0x1;
            } else {

                r0 = 0x0;
            }
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                if (f2 != f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                if (f1 != f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F2BE8 - 0x800F2BE8 | size: 0x410 */
void fn_800F2BE8(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r0 - r3;
            r0 = __cntlzw(r0);
            r0 = (u32)r0 >> 5;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r3, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            if (f1 == f0) {
                r0 = 0x1;
            } else {

                r0 = 0x0;
            }
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                if (f2 == f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                if (f1 == f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F2FF8 - 0x800F2FF8 | size: 0x420 */
void fn_800F2FF8(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r4 = (s32)r5 >> 31;
            r3 = (u32)r0 >> 31;
            r0 = r5 - r0;
            r0 = r4 + r3; /* +carry */;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r5, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            /* cror eq, gt, eq */;
            if (f1 == f0) {
                r0 = 0x1;
            } else {

                r0 = 0x0;
            }
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                /* cror eq, gt, eq */;
                if (f2 == f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                /* cror eq, gt, eq */;
                if (f1 == f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F3418 - 0x800F3418 | size: 0x418 */
void fn_800F3418(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r4 ^ r0;
            r3 = (s32)r0 >> 1;
            r0 = r0 & r4;
            r0 = r3 - r0;
            r0 = (u32)r0 >> 31;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r4, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            if (f1 > f0) {
                r0 = 0x1;
            } else {

                r0 = 0x0;
            }
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                if (f2 > f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                if (f1 > f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F3830 - 0x800F3830 | size: 0x420 */
void fn_800F3830(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r3 = (u32)r5 >> 31;
            r4 = (s32)r0 >> 31;
            r0 = r0 - r5;
            r0 = r4 + r3; /* +carry */;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r5, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            /* cror eq, lt, eq */;
            if (f1 == f0) {
                r0 = 0x1;
            } else {

                r0 = 0x0;
            }
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                /* cror eq, lt, eq */;
                if (f2 == f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                /* cror eq, lt, eq */;
                if (f1 == f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F3C50 - 0x800F3C50 | size: 0x418 */
void fn_800F3C50(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r4 ^ r3;
            r3 = (s32)r0 >> 1;
            r0 = r0 & r4;
            r0 = r3 - r0;
            r0 = (u32)r0 >> 31;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r3, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            if (f1 < f0) {
                r0 = 0x1;
            } else {

                r0 = 0x0;
            }
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                if (f2 < f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                if (f1 < f0) {
                    r0 = 0x1;
                } else {

                    r0 = 0x0;
                }
                *(u32*)(sp + 0x38) = r0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F4068 - 0x800F4068 | size: 0x3D8 */
void fn_800F4068(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r3 - r0;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r3, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            f0 = f1 - f0;
            *(f32*)(sp + 0x38) = f0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                f0 = f2 - f0;
                *(f32*)(sp + 0x38) = f0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                f0 = f1 - f0;
                *(f32*)(sp + 0x38) = f0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F4440 - 0x800F4440 | size: 0x3D8 */
void fn_800F4440(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r3 + r0;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r3, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            f0 = f1 + f0;
            *(f32*)(sp + 0x38) = f0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                f0 = f2 + f0;
                *(f32*)(sp + 0x38) = f0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                f0 = f1 + f0;
                *(f32*)(sp + 0x38) = f0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F4818 - 0x800F4818 | size: 0x420 */
void fn_800F4818(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800CE318();
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    if ((s32)r3 != (s32)0x0) {
        r0 = r29 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (s32)r4 / (s32)r3;
                r0 = r0 * r3;
                r0 = r4 - r0;
                *(u32*)(sp + 0x38) = r0;

            } else {
                /* xoris r3, r4, 0x8000 */;
                r0 = (0x4330 << 16);
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x48) = r0;
                f2 = *(f32*)(sp + 0x3C);
                f0 = *(f64*)(sp + 0x48);
                f1 = f0 - f1;
                fn_800CE318();
                f0 = (f32)f1;
                *(f32*)(sp + 0x38) = f0;

            }
        } else {
                r0 = r28 & 0x3F;
                if ((u32)r0 == (u32)0x2) {
                    /* xoris r3, r4, 0x8000 */;
                    r0 = (0x4330 << 16);
                    f1 = *(f64*)lbl_8047CCC0;
                    *(u32*)(sp + 0x48) = r0;
                    f2 = *(f32*)(sp + 0x3C);
                    f0 = *(f64*)(sp + 0x48);
                    f1 = f0 - f1;
                    fn_800CE318();
                    f0 = (f32)f1;
                    *(f32*)(sp + 0x38) = f0;

                } else {
                    /* xoris r3, r4, 0x8000 */;
                    r0 = (0x4330 << 16);
                    f1 = *(f64*)lbl_8047CCC0;
                    *(u32*)(sp + 0x48) = r0;
                    f2 = *(f32*)(sp + 0x3C);
                    f0 = *(f64*)(sp + 0x48);
                    f1 = f0 - f1;
                    fn_800CE318();
                    f0 = (f32)f1;
                    *(f32*)(sp + 0x38) = f0;

                }
            }
        } else {
                    r3 = r31 + 0xd8;
                    /* crclr cr1eq */;
                    fn_800DD38C();
                    r0 = 0x0;
                    *(u32*)(sp + 0x38) = r0;
        }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F4C38 - 0x800F4C38 | size: 0x3F4 */
void fn_800F4C38(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    if ((s32)r3 != (s32)0x0) {
        r0 = r29 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (s32)r4 / (s32)r3;
                *(u32*)(sp + 0x38) = r0;

            } else {
                /* xoris r3, r4, 0x8000 */;
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x48) = r0;
                f0 = *(f32*)(sp + 0x3C);
                f1 = *(f64*)(sp + 0x48);
                f1 = f1 - f2;
                f0 = f1 / f0;
                *(f32*)(sp + 0x38) = f0;

            }
        } else {
                r0 = r28 & 0x3F;
                if ((u32)r0 == (u32)0x2) {
                    /* xoris r3, r3, 0x8000 */;
                    r0 = (0x4330 << 16);
                    f1 = *(f64*)lbl_8047CCC0;
                    *(u32*)(sp + 0x48) = r0;
                    f2 = *(f32*)(sp + 0x40);
                    f0 = *(f64*)(sp + 0x48);
                    f0 = f0 - f1;
                    f0 = f2 / f0;
                    *(f32*)(sp + 0x38) = f0;

                } else {
                    f1 = *(f32*)(sp + 0x40);
                    f0 = *(f32*)(sp + 0x3C);
                    f0 = f1 / f0;
                    *(f32*)(sp + 0x38) = f0;

                }
            }
        } else {
                    r3 = r31 + 0xd8;
                    /* crclr cr1eq */;
                    fn_800DD38C();
                    r0 = 0x0;
                    *(u32*)(sp + 0x38) = r0;
        }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F502C - 0x800F502C | size: 0x3D8 */
void fn_800F502C(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r3 * r0;
            *(u32*)(sp + 0x38) = r0;

        } else {
            /* xoris r3, r3, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047CCC0;
            *(u32*)(sp + 0x48) = r0;
            f0 = *(f32*)(sp + 0x3C);
            f1 = *(f64*)(sp + 0x48);
            f1 = f1 - f2;
            f0 = f1 * f0;
            *(f32*)(sp + 0x38) = f0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x48) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CCC0;
                *(u32*)(sp + 0x4C) = r0;
                f2 = *(f32*)(sp + 0x40);
                f0 = *(f64*)(sp + 0x48);
                f0 = f0 - f1;
                f0 = f2 * f0;
                *(f32*)(sp + 0x38) = f0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                f0 = f1 * f0;
                *(f32*)(sp + 0x38) = f0;
                }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F5404 - 0x800F5404 | size: 0x1D8 */
void fn_800F5404(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r29 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r29 + 0x14) = r0;
    r4 = *(u8*)((u8*)r3 + 0x0);
    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x14) = r0;
    if ((u32)r4 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r4 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x1C) = r0;

    } else {
        r0 = r4 & 0x00000080;
        r30 = r4 & 0xFFFF;
        if ((u32)r4 != (u32)0x0) {
            r3 = *(u32*)((u8*)r29 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r29 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r29 + 0x28) = r3;
                r3 = r29 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r29 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r29 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r29 + 0x28) = r3;
                r3 = r29 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r29 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r29 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r29 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r29 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r29 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r29 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x1C) = r0;
    }

    r4 = *(u32*)((u8*)r29 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F55DC - 0x800F55DC | size: 0x214 */
void fn_800F55DC(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r4 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r28 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r28 + 0x14) = r0;
    r29 = *(u8*)((u8*)r3 + 0x0);
    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x14) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x1C) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r28 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r28 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r28 + 0x28) = r3;
                r3 = r28 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r28 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r28 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r28 + 0x28) = r3;
                r3 = r28 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r28 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r28 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r28 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r28 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r28 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r28 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x1C) = r0;
    }

    r0 = r29 & 0x3F;
    if ((s32)r0 == (s32)0x2) {
        r0 = -r3;
        *(u32*)(sp + 0x20) = r0;
    }
    r0 = r29 & 0x3F;
    if ((s32)r0 != (s32)0x2) {
        f0 = *(f32*)(sp + 0x24);
        f0 = -f0;
        *(f32*)(sp + 0x20) = f0;
    }
    r4 = *(u32*)((u8*)r28 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r28 + 0x28) = r3;
        r3 = r28 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F57F0 - 0x800F57F0 | size: 0x24C */
void fn_800F57F0(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047CCBC[];
    extern u8 lbl_8047CCC0[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r4 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r28 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r28 + 0x14) = r0;
    r29 = *(u8*)((u8*)r3 + 0x0);
    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x14) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x1C) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r28 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r28 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r28 + 0x28) = r3;
                r3 = r28 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r28 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r28 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r28 + 0x28) = r3;
                r3 = r28 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r28 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r28 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r28 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r28 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r28 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r28 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x1C) = r0;
    }

    r0 = r29 & 0x3F;
    if ((s32)r0 == (s32)0x2) {
        r0 = __cntlzw(r3);
        r0 = (u32)r0 >> 5;
        *(u32*)(sp + 0x20) = r0;
    }
    r0 = r29 & 0x3F;
    if ((s32)r0 != (s32)0x2) {
        f1 = *(f32*)(sp + 0x24);
        f0 = *(f32*)lbl_8047CCBC;
        r3 = 0; /* mfcr */;
        r0 = (0x4330 << 16);
        /* extrwi r3, r3, 1, 2 */;
        *(u32*)(sp + 0x28) = r0;
        r0 = r3 ^ 0x1;
        f1 = *(f64*)lbl_8047CCC0;
        r0 = __cntlzw(r0);
        r0 = (u32)r0 >> 5;
        /* xoris r0, r0, 0x8000 */;
        *(u32*)(sp + 0x2C) = r0;
        f0 = *(f64*)(sp + 0x28);
        f0 = f0 - f1;
        *(f32*)(sp + 0x20) = f0;
    }
    r4 = *(u32*)((u8*)r28 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r28 + 0x28) = r3;
        r3 = r28 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F5A3C - 0x800F5A3C | size: 0x264 */
void fn_800F5A3C(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r25 = r3;
    r3 = (u32)lbl_80271068;
    r29 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r25 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r25 + 0x14) = r0;
    r3 = *(u32*)((u8*)r25 + 0x14);
    r26 = *(u8*)((u8*)r4 + 0x0);
    r27 = *(u16*)((u8*)r3 + 0x0);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r25 + 0x14) = r0;
    r3 = *(u32*)((u8*)r25 + 0x28);
    if ((s32)r3 <= (s32)0x0) {
        r3 = r29 + 0x14;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r25 + 0x6C);
        *(u32*)(sp + 0x20) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r25 + 0x28) = r3;
        r3 = r25 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x20) = r0;
    }
    r31 = r26 & 0xFFFF;
    r28 = 0x0;
    while ((s32)r28 < (s32)r27) {
            r3 = *(u32*)((u8*)r25 + 0x28);
            r4 = r30 + r28;
            if ((s32)r3 > (s32)0x40) {
                r3 = r29 + 0x0;
                /* crclr cr1eq */;
                fn_800DD38C();
            } else {

                r0 = r3 + 0x1;
                r3 = r3 << 2;
                *(u32*)((u8*)r25 + 0x28) = r0;
                r0 = r3 + 0x6c;
                *(u32*)(r25 + r0) = r4;
            }
            r0 = *(u32*)lbl_8047E710;
            *(u32*)(sp + 0x14) = r0;
            if ((u32)r31 == (u32)0x0) {
                r4 = r31;
                r3 = r29 + 0x28;
                /* crclr cr1eq */;
                fn_800DD38C();
                *(u32*)(sp + 0x1C) = r0;

            } else {
                r0 = r26 & 0x00000080;
                if ((u32)r31 != (u32)0x0) {
                    r3 = *(u32*)((u8*)r25 + 0x28);
                    if ((s32)r3 <= (s32)0x0) {
                        r3 = r29 + 0x14;
                        /* crclr cr1eq */;
                        fn_800DD38C();
                        r0 = *(u32*)((u8*)r25 + 0x6C);
                        *(u32*)(sp + 0x10) = r0;
                    } else {

                        /* subi r0, r3, 0x1 */;
                        r3 = r0 << 2;
                        *(u32*)((u8*)r25 + 0x28) = r0;
                        r0 = r3 + 0x6c;
                        r0 = *(u32*)(r25 + r0);
                        *(u32*)(sp + 0x10) = r0;
                    }
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r25 + 0x28);
                    if ((s32)r3 <= (s32)0x0) {
                        r3 = r29 + 0x14;
                        /* crclr cr1eq */;
                        fn_800DD38C();
                        r0 = *(u32*)((u8*)r25 + 0x6C);
                        *(u32*)(sp + 0xC) = r0;
                    } else {

                        /* subi r0, r3, 0x1 */;
                        r3 = r0 << 2;
                        *(u32*)((u8*)r25 + 0x28) = r0;
                        r0 = r3 + 0x6c;
                        r0 = *(u32*)(r25 + r0);
                        *(u32*)(sp + 0xC) = r0;
                    }
                    r0 = r31 & 0x00000020;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = r31 & 0x00000040;
                        if ((s32)r3 != (s32)0x0) {
                            r0 = *(u32*)((u8*)r25 + 0x1C);
                            r0 = r0 + r4;
                            r3 = r0 << 2;
                            r0 = r3 + 0x6c;
                            r0 = r25 + r0;
                            *(u32*)(sp + 0x8) = r0;

                        } else {
                            r3 = *(u32*)((u8*)r25 + 0x18);
                            r0 = r4 << 2;
                            r0 = r3 + r0;
                            *(u32*)(sp + 0x8) = r0;

                        }
                    } else {
                            r0 = r31 & 0x00000040;
                            if ((s32)r3 != (s32)0x0) {
                                r0 = *(u32*)((u8*)r25 + 0x1C);
                                r0 = r0 + r4;
                                r3 = r0 << 2;
                                r3 = r3 + 0x6c;
                                r3 = r25 + r3;
                            } else {

                                r3 = *(u32*)((u8*)r25 + 0x18);
                                r0 = r4 << 2;
                                r3 = r3 + r0;
                            }
                            r0 = r31 & 0x00000100;
                            if ((s32)r3 != (s32)0x0) {

                            } else {
                                r0 = *(u32*)((u8*)r3 + 0x0);
                                *(u32*)(sp + 0x8) = r0;
                                }
                            }
                            }

                *(u32*)(sp + 0x1C) = r0;
            }

            r3 = *(u32*)((u8*)r25 + 0x28);
            if ((s32)r3 > (s32)0x40) {
                r3 = r29 + 0x0;
                /* crclr cr1eq */;
                fn_800DD38C();
            } else {

                r0 = r3 + 0x1;
                r3 = r3 << 2;
                *(u32*)((u8*)r25 + 0x28) = r0;
                r3 = r3 + 0x6c;
                *(u32*)(r25 + r3) = r0;
            }
            r28 = r28 + 0x1;
    }
    r3 = 0x1;
    return;
}

/* fn_800F5CA0 - 0x800F5CA0 | size: 0x24C */
void fn_800F5CA0(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r3 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r29 + 0x14);
    r31 = (u32)lbl_80271068;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r29 + 0x14) = r0;
    r3 = *(u32*)((u8*)r29 + 0x28);
    r30 = *(u8*)((u8*)r4 + 0x0);
    if ((s32)r3 <= (s32)0x0) {
        r3 = r31 + 0x14;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r29 + 0x6C);
        *(u32*)(sp + 0x20) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x20) = r0;
    }
    r4 = *(u32*)((u8*)r29 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r5;
    }
    r3 = r30 | 0x100;
    r0 = *(u32*)lbl_8047E710;
    r30 = r3 & 0xFFFF;
    *(u32*)(sp + 0x14) = r0;
    if ((s32)r4 == (s32)0x40) {
        r4 = r30;
        r3 = r31 + 0x28;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x1C) = r0;

    } else {
        r0 = r30 & 0x00000080;
        if ((s32)r4 != (s32)0x40) {
            r3 = *(u32*)((u8*)r29 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r29 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r29 + 0x28) = r3;
                r3 = r29 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r29 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r29 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r29 + 0x28) = r3;
                r3 = r29 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0xC) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r29 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r29 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r29 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r29 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r29 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r29 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x1C) = r0;
    }

    r4 = *(u32*)((u8*)r29 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r29 + 0x28) = r3;
        r3 = r29 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F5EEC - 0x800F5EEC | size: 0x3D0 */
void fn_800F5EEC(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r3 | r0;
            *(u32*)(sp + 0x38) = r0;

        } else {
            f0 = *(f32*)(sp + 0x3C);
            f0 = (f64)(s32)f0;
            *(f64*)(sp + 0x48) = f0;
            r0 = r3 | r0;
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                f0 = *(f32*)(sp + 0x40);
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x48) = f0;
                r0 = r3 | r0;
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                f1 = (f64)(s32)f1;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x48) = f1;
                *(f64*)(sp + 0x50) = f0;
                r0 = r3 | r0;
                *(u32*)(sp + 0x38) = r0;
            }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F62BC - 0x800F62BC | size: 0x3D0 */
void fn_800F62BC(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8047E710[];
    extern void fn_800DD38C();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r27 = r3;
    r3 = (u32)lbl_80271068;
    r31 = (u32)lbl_80271068;
    r4 = *(u32*)((u8*)r27 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r28 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)lbl_8047E710;
    r29 = *(u8*)((u8*)r3 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r28 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r28 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x34) = r0;

    } else {
        r0 = r28 & 0x00000080;
        r30 = r28 & 0xFFFF;
        if ((u32)r28 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x1C) = r0;
            }
            *(u32*)(sp + 0xC) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x18) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0xC) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0xC) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0xC) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x34) = r0;
    }

    r0 = *(u32*)lbl_8047E710;
    *(u32*)(sp + 0x20) = r0;
    if ((u32)r29 == (u32)0x0) {
        r3 = r31 + 0x28;
        r4 = r29 & 0xFFFF;
        /* crclr cr1eq */;
        fn_800DD38C();
        *(u32*)(sp + 0x28) = r0;

    } else {
        r0 = r29 & 0x00000080;
        r30 = r29 & 0xFFFF;
        if ((u32)r29 != (u32)0x0) {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x14) = r0;
            }
            *(u32*)(sp + 0x8) = r0;

        } else {
            r3 = *(u32*)((u8*)r27 + 0x28);
            if ((s32)r3 <= (s32)0x0) {
                r3 = r31 + 0x14;
                /* crclr cr1eq */;
                fn_800DD38C();
                r0 = *(u32*)((u8*)r27 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            } else {

                /* subi r3, r3, 0x1 */;
                r0 = r3 << 2;
                *(u32*)((u8*)r27 + 0x28) = r3;
                r3 = r27 + r0;
                r0 = *(u32*)((u8*)r3 + 0x6C);
                *(u32*)(sp + 0x10) = r0;
            }
            r0 = r30 & 0x00000020;
            if ((s32)r3 != (s32)0x0) {
                r0 = r30 & 0x00000040;
                if ((s32)r3 != (s32)0x0) {
                    r0 = *(u32*)((u8*)r27 + 0x1C);
                    r0 = r0 + r4;
                    r3 = r0 << 2;
                    r0 = r3 + 0x6c;
                    r0 = r27 + r0;
                    *(u32*)(sp + 0x8) = r0;

                } else {
                    r3 = *(u32*)((u8*)r27 + 0x18);
                    r0 = r4 << 2;
                    r0 = r3 + r0;
                    *(u32*)(sp + 0x8) = r0;

                }
            } else {
                    r0 = r30 & 0x00000040;
                    if ((s32)r3 != (s32)0x0) {
                        r0 = *(u32*)((u8*)r27 + 0x1C);
                        r0 = r0 + r4;
                        r3 = r0 << 2;
                        r3 = r3 + 0x6c;
                        r3 = r27 + r3;
                    } else {

                        r3 = *(u32*)((u8*)r27 + 0x18);
                        r0 = r4 << 2;
                        r3 = r3 + r0;
                    }
                    r0 = r30 & 0x00000100;
                    if ((s32)r3 != (s32)0x0) {

                    } else {
                        r0 = *(u32*)((u8*)r3 + 0x0);
                        *(u32*)(sp + 0x8) = r0;
                        }
                    }
                    }

        *(u32*)(sp + 0x28) = r0;
    }

    r0 = r29 & 0x3F;
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0x3F;
        if ((u32)r0 == (u32)0x2) {
            r0 = r3 & r0;
            *(u32*)(sp + 0x38) = r0;

        } else {
            f0 = *(f32*)(sp + 0x3C);
            f0 = (f64)(s32)f0;
            *(f64*)(sp + 0x48) = f0;
            r0 = r3 & r0;
            *(u32*)(sp + 0x38) = r0;

        }
    } else {
            r0 = r28 & 0x3F;
            if ((u32)r0 == (u32)0x2) {
                f0 = *(f32*)(sp + 0x40);
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x48) = f0;
                r0 = r3 & r0;
                *(u32*)(sp + 0x38) = r0;

            } else {
                f1 = *(f32*)(sp + 0x40);
                f0 = *(f32*)(sp + 0x3C);
                f1 = (f64)(s32)f1;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x48) = f1;
                *(f64*)(sp + 0x50) = f0;
                r0 = r3 & r0;
                *(u32*)(sp + 0x38) = r0;
            }
            }

    r4 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r27 + 0x28) = r3;
        r3 = r27 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F668C - 0x800F668C | size: 0x80
 * GSthreadReadNextBytecode - Read the next bytecode value from the
 * script's instruction pointer and push it onto the stack.
 * thread+0x14 = instruction pointer, thread+0x28 = stack count,
 * stack data starts at thread+0x6C.
 */
u32 fn_800F668C(u8* thread) {
    extern u8 lbl_80271068[];
    extern void fn_800DD38C(const char* msg);
    u32* ip;
    u32 value;
    s32 stackCount;

    /* Read value from instruction stream and advance IP */
    ip = (u32*)*(u32*)(thread + 0x14);
    *(u32*)(thread + 0x14) = (u32)(ip + 1);

    value = *ip;
    stackCount = *(s32*)(thread + 0x28);

    if (stackCount > 0x40) {
        fn_800DD38C((const char*)lbl_80271068);
    } else {
        *(u32*)(thread + 0x28) = stackCount + 1;
        *(u32*)(thread + 0x6C + stackCount * 4) = value;
    }

    /* Advance IP by another 4 bytes (skip operand) */
    *(u32*)(thread + 0x14) = *(u32*)(thread + 0x14) + 4;
    return 1;
}

/* fn_800F670C - 0x800F670C | size: 0xA0
 * GSthreadBranchIfZero - Pop the stack and branch if zero.
 * If the popped value is nonzero, skip the branch offset (IP += 4).
 * If zero, read the branch offset and compute the new IP.
 */
u32 fn_800F670C(u8* thread) {
    extern u8 lbl_8027107C[];
    extern void fn_800DD38C(const char* msg);
    s32 stackCount;
    u32 value;

    stackCount = *(s32*)(thread + 0x28);

    if (stackCount <= 0) {
        fn_800DD38C((const char*)lbl_8027107C);
        value = *(u32*)(thread + 0x6C);
    } else {
        stackCount--;
        *(u32*)(thread + 0x28) = (u32)stackCount;
        value = *(u32*)(thread + 0x6C + stackCount * 4);
    }

    if (value != 0) {
        /* Skip the branch offset operand */
        *(u32*)(thread + 0x14) = *(u32*)(thread + 0x14) + 4;
    } else {
        /* Read offset and compute target address */
        u32 ip = *(u32*)(thread + 0x14);
        u32 base = *(u32*)(thread + 0x0);
        u32 offset = *(u32*)ip;
        *(u32*)(thread + 0x14) = base + offset;
    }
    return 1;
}

/* fn_800F67AC - 0x800F67AC | size: 0x1C */
void fn_800F67AC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = *(u32*)((u8*)r3 + 0x14);
    r5 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u32*)((u8*)r4 + 0x0);
    r0 = r5 + r0;
    *(u32*)((u8*)r3 + 0x14) = r0;
    r3 = 0x1;
    return;
}

/* fn_800F67C8 - 0x800F67C8 | size: 0x184 */
void fn_800F67C8(void) {
    extern u8 lbl_8027107C[];
    extern void fn_800DD38C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x28);
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0x10) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x10) = r0;
    }
    r3 = *(u32*)((u8*)r31 + 0x28);
    *(u32*)(sp + 0x1C) = r0;
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0xC) = r0;
    }
    *(u32*)((u8*)r31 + 0x1C) = r0;
    r3 = *(u32*)((u8*)r31 + 0x28);
    *(u32*)(sp + 0x18) = r0;
    if ((s32)r3 <= (s32)0x0) {
        r3 = (u32)lbl_8027107C;
        r3 = (u32)lbl_8027107C;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    }
    r3 = (u32)lbl_8027107C;
    r29 = (u32)lbl_8027107C;
    *(u32*)(sp + 0x14) = r0;
    r28 = 0x0;
    *(u32*)((u8*)r31 + 0x14) = r0;
    while ((s32)r28 < (s32)r30) {

        r3 = *(u32*)((u8*)r31 + 0x28);
        if ((s32)r3 <= (s32)0x0) {
            r3 = r29;
            /* crclr cr1eq */;
            fn_800DD38C();
        } else {

            /* subi r0, r3, 0x1 */;
            *(u32*)((u8*)r31 + 0x28) = r0;
        }
        r28 = r28 + 0x1;

    }
    r0 = *(u32*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) {
        r0 = 0x3;
        r3 = 0x0;
        *(u8*)((u8*)r31 + 0x4) = r0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* fn_800F694C - 0x800F694C | size: 0x168 */
void fn_800F694C(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_8027115C[];
    extern void fn_800DD38C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r31 = *(u32*)((u8*)r3 + 0x0);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r28 + 0x14) = r0;
    r3 = *(u32*)((u8*)r28 + 0x14);
    r30 = *(u16*)((u8*)r3 + 0x0);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r28 + 0x14) = r0;
    r3 = *(u32*)((u8*)r28 + 0x14);
    r29 = *(u16*)((u8*)r3 + 0x0);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r28 + 0x14) = r0;
    r4 = *(u32*)((u8*)r28 + 0x28);
    r5 = *(u32*)((u8*)r28 + 0x14);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r28 + 0x28) = r3;
        r3 = r28 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r5;
    }
    r4 = *(u32*)((u8*)r28 + 0x28);
    r5 = *(u32*)((u8*)r28 + 0x1C);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r28 + 0x28) = r3;
        r3 = r28 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r5;
    }
    r4 = *(u32*)((u8*)r28 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = (u32)lbl_80271068;
        r3 = (u32)lbl_80271068;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r28 + 0x28) = r3;
        r3 = r28 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r29;
    }
    r3 = r29 + 0x3;
    r0 = *(u32*)((u8*)r28 + 0x28);
    r3 = r30 + r3;
    r0 = r0 - r3;
    *(u32*)((u8*)r28 + 0x1C) = r0;
    r4 = *(u32*)((u8*)r28 + 0x0);
    r0 = *(u16*)((u8*)r4 + 0x4);
    if ((u32)r31 >= (u32)r0) {
        r3 = (u32)lbl_8027115C;
        r4 = r31;
        r3 = (u32)lbl_8027115C;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r0 = r31 << 2;
        r3 = r4 + r0;
        r0 = *(u32*)((u8*)r3 + 0x18);
        r0 = r4 + r0;
        *(u32*)((u8*)r28 + 0x14) = r0;
    }
    r3 = 0x1;
    return;
}

/* fn_800F6AB4 - 0x800F6AB4 | size: 0xA0
 * GSthreadPopN - Pop N values from the thread's stack.
 * Reads a 1-byte skip, then a 16-bit count from the instruction
 * stream, and decrements the stack count that many times.
 */
u32 fn_800F6AB4(u8* thread) {
    extern u8 lbl_8027107C[];
    extern void fn_800DD38C(const char* msg);
    u32 ip;
    u16 popCount;
    s32 i;

    /* Skip one byte */
    ip = *(u32*)(thread + 0x14);
    *(u32*)(thread + 0x14) = ip + 1;

    /* Read 16-bit pop count */
    ip = *(u32*)(thread + 0x14);
    popCount = *(u16*)ip;
    *(u32*)(thread + 0x14) = ip + 2;

    for (i = 0; i < (s32)popCount; i++) {
        s32 count = *(s32*)(thread + 0x28);
        if (count <= 0) {
            fn_800DD38C((const char*)lbl_8027107C);
        } else {
            *(u32*)(thread + 0x28) = (u32)(count - 1);
        }
    }
    return 1;
}

/* fn_800F6B54 - 0x800F6B54 | size: 0x58
 * GSthreadPushMarker - Push a -1 marker onto the thread's stack.
 * The stack is an array of u32 starting at offset 0x6C, with the
 * count at offset 0x28. Asserts if count exceeds 0x40.
 */
u32 fn_800F6B54(u8* thread) {
    extern u8 lbl_80271068[];
    extern void fn_800DD38C(const char* msg);
    s32 count;

    count = *(s32*)(thread + 0x28);
    if (count > 0x40) {
        fn_800DD38C((const char*)lbl_80271068);
    } else {
        *(u32*)(thread + 0x28) = count + 1;
        *(u32*)(thread + 0x6C + count * 4) = (u32)-1;
    }
    return 1;
}

/* fn_800F6BAC - 0x800F6BAC | size: 0x10 */
void fn_800F6BAC(void) {
    u32 r0 = 0;
    u32 r3 = 0;

    r0 = 0x3;
    *(u8*)((u8*)r3 + 0x4) = r0;
    r3 = 0x0;
    return;
}

/* fn_800F6BC4 - 0x800F6BC4 | size: 0x154 */
void fn_800F6BC4(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_803155D0[];
    extern void fn_800DD38C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)lbl_80271068;
    r31 = r3;
    r30 = (u32)lbl_80271068;
    while (1) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        if ((u32)r0 == (u32)0x0) {
            r4 = *(u32*)((u8*)r31 + 0x8);
            r3 = r30 + 0x120;
            /* crclr cr1eq */;
            ((void(*)(void))fn_800DD970)();

        } else if ((u32)r0 == (u32)0x3) {
            r0 = 0x0;
            *(u8*)((u8*)r31 + 0x4) = r0;

        }
        r3 = *(u32*)((u8*)r31 + 0x14);
        r0 = r3 + 0x1;
        *(u32*)((u8*)r31 + 0x14) = r0;
        r4 = *(u8*)((u8*)r3 + 0x0);
        if ((s32)r4 >= (s32)0x26) {
            r3 = r30 + 0x150;
            /* crclr cr1eq */;
            fn_800DD38C();

        } else {
            r3 = (u32)lbl_803155D0;
            /* clrlslwi r0, r4, 24, 2 */;
            r3 = (u32)lbl_803155D0;
            r12 = *(u32*)(r3 + r0);
            if ((u32)r12 != (u32)0x0) {
                r3 = r31;
                ctr_fn = (void(*)(void))r12;
                ctr_fn();
            }
        }

        r3 = *(u32*)((u8*)r31 + 0x28);
        if ((s32)r3 <= (s32)0x0) continue;
        /* subic. r3, r3, 0x1 */;
        r0 = r3 + 0x1;
        ctr_fn = (void(*)(void))r0;
        if ((s32)r3 < (s32)0x0) continue;
        do {
            /* subi r3, r3, 0x1 */;
        } while (--ctr != 0);
        continue;
    }

    r0 = *(u8*)((u8*)r31 + 0x4);
    if ((u32)r0 == (u32)0x4) {
        r0 = 0x0;
        *(u8*)((u8*)r31 + 0x4) = r0;
    }
    r3 = *(u32*)((u8*)r31 + 0x28);
    if ((s32)r3 <= (s32)0x0) {
        r3 = r30 + 0x14;
        /* crclr cr1eq */;
        fn_800DD38C();
        r0 = *(u32*)((u8*)r31 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    } else {

        /* subi r3, r3, 0x1 */;
        r0 = r3 << 2;
        *(u32*)((u8*)r31 + 0x28) = r3;
        r3 = r31 + r0;
        r0 = *(u32*)((u8*)r3 + 0x6C);
        *(u32*)(sp + 0x8) = r0;
    }
    r12 = *(u32*)((u8*)r31 + 0x10);
    if ((u32)r12 != (u32)0x0) {
        r3 = r31;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    return;
}

/* fn_800F6D18 - 0x800F6D18 | size: 0x350 */
void fn_800F6D18(void) {
    extern u8 lbl_80271068[];
    extern u8 lbl_80478B00[];
    extern void fn_800DD38C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r7 = 0x0;
    r6 = (u32)lbl_80271068;
    r0 = r7;
    r8 = 0x0;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r7;
    r31 = (u32)lbl_80271068;
    while (1) {
        r5 = *(u32*)lbl_80478B00;
        r6 = *(u16*)((u8*)r5 + 0x0);
        if ((u32)r8 >= (u32)r6) break;
        r3 = *(u32*)((u8*)r5 + 0xC);
        r5 = r3 + r7;
        r3 = *(u8*)((u8*)r5 + 0x4);
        if ((u32)r3 == (u32)0x4) {
            *(u8*)((u8*)r5 + 0x4) = r4;
        }
        r3 = *(u8*)((u8*)r5 + 0x4);
        if ((u32)r3 == (u32)0x3) {
            *(u8*)((u8*)r5 + 0x4) = r0;
        }
        r7 = r7 + 0x16c;
        r8 = r8 + 0x1;

    }
    r4 = 0x0;
    r30 = *(u32*)((u8*)r5 + 0xC);
    r3 = r4;
    ctr_fn = (void(*)(void))r6;
    if ((u32)r6 > (u32)0x0) {
        do {
            r0 = *(u32*)((u8*)r5 + 0xC);
            r30 = r0 + r3;
            r0 = *(u8*)((u8*)r30 + 0x4);
            if ((u32)r0 == (u32)0x0) break;
            r3 = r3 + 0x16c;
            r4 = r4 + 0x1;
        } while (--ctr != 0);
    }
    if ((u32)r4 == (u32)r6) {
        r4 = r26;
        r3 = r31 + 0x178;
        /* crclr cr1eq */;
        fn_800DD38C();
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0x16c;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = *(u32*)lbl_80478B00;
    r4 = (u32)r26 >> 16;
    r3 = *(u32*)((u8*)r3 + 0x8);
    while ((u32)r3 != (u32)0x0) {
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                *(u32*)((u8*)r30 + 0x0) = r3;
                r0 = *(u32*)((u8*)r3 + 0x10);
                r0 = r3 + r0;
                *(u32*)((u8*)r30 + 0x18) = r0;
                break;
            }
            r3 = *(u32*)((u8*)r3 + 0x14);
    }

    if ((u32)r3 == (u32)0x0) {
        r4 = r26;
        r3 = r31 + 0x1b4;
        /* crclr cr1eq */;
        fn_800DD38C();
        r3 = 0x0;
        return;
    }
    r7 = *(u32*)lbl_80478B00;
    r6 = *(u16*)((u8*)r7 + 0x4);
    r29 = r6;
    while (1) {
        r0 = r29 + 0x1;
        r0 = r0 & 0xFFFF;
        r29 = r0;
        if ((u32)r0 == (u32)r6) {
            r4 = r26;
            r3 = r31 + 0x1f0;
            /* crclr cr1eq */;
            fn_800DD38C();
            r3 = 0x0;
            return;
        }
        if ((u32)r0 == (u32)0x0) continue;
        r3 = *(u16*)((u8*)r7 + 0x0);
        r0 = r0 & 0xFFFF;
        r5 = 0x0;
        ctr_fn = (void(*)(void))r3;
        if ((s32)r3 > (s32)0x0) {
            do {
                r3 = *(u32*)((u8*)r7 + 0xC);
                r4 = r3 + r5;
                r3 = *(u8*)((u8*)r4 + 0x4);
                if ((u32)r3 == (u32)0x0) continue;
                r3 = *(u16*)((u8*)r4 + 0x6);
                if ((u32)r3 != (u32)r0) continue;
            } while (--ctr != 0);
        }
        if ((u32)r4 != (u32)0x0) continue;
        break;
    }
    *(u16*)((u8*)r7 + 0x4) = r29;
    r4 = r26 & 0xFFFF;
    r5 = *(u32*)((u8*)r30 + 0x0);
    r0 = *(u16*)((u8*)r5 + 0x4);
    if ((u32)r4 >= (u32)r0) {
        r3 = r31 + 0xf4;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r0 = r4 << 2;
        r3 = r5 + r0;
        r0 = *(u32*)((u8*)r3 + 0x18);
        r0 = r5 + r0;
        *(u32*)((u8*)r30 + 0x14) = r0;
    }
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x4) = r0;
    *(u32*)((u8*)r30 + 0x8) = r26;
    *(u16*)((u8*)r30 + 0x6) = r29;
    r4 = *(u32*)((u8*)r30 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r30 + 0x28) = r3;
        r3 = r30 + r0;
        r0 = 0x0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r26 = 0x0;
    while ((u32)r26 < (u32)r27) {

        r3 = r28;
        r4 = 0x1;
        __va_arg();
        r4 = *(u32*)((u8*)r30 + 0x28);
        r5 = *(u32*)((u8*)r3 + 0x0);
        if ((s32)r4 > (s32)0x40) {
            r3 = r31 + 0x0;
            /* crclr cr1eq */;
            fn_800DD38C();
        } else {

            r0 = r4 + 0x1;
            r3 = r4 << 2;
            *(u32*)((u8*)r30 + 0x28) = r0;
            r0 = r3 + 0x6c;
            *(u32*)(r30 + r0) = r5;
        }
        r26 = r26 + 0x1;

    }
    r4 = *(u32*)((u8*)r30 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r30 + 0x28) = r3;
        r3 = r30 + r0;
        r0 = 0x0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r4 = *(u32*)((u8*)r30 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r30 + 0x28) = r3;
        r3 = r30 + r0;
        r0 = 0x0;
        *(u32*)((u8*)r3 + 0x6C) = r0;
    }
    r4 = *(u32*)((u8*)r30 + 0x28);
    if ((s32)r4 > (s32)0x40) {
        r3 = r31 + 0x0;
        /* crclr cr1eq */;
        fn_800DD38C();
    } else {

        r3 = r4 + 0x1;
        r0 = r4 << 2;
        *(u32*)((u8*)r30 + 0x28) = r3;
        r3 = r30 + r0;
        *(u32*)((u8*)r3 + 0x6C) = r27;
    }
    r3 = r30;

    return;
}

/* fn_800F7068 - 0x800F7068 | size: 0xA0 */
void fn_800F7068(void) {
    extern u8 lbl_80478B00[];
    extern void fn_800F0308();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r4 & 0xFF;
    r30 = r3 & 0xFFFF;
    while (1) {
        r5 = *(u32*)lbl_80478B00;
        r4 = 0x0;
        r0 = *(u16*)((u8*)r5 + 0x0);
        ctr_fn = (void(*)(void))r0;
        if ((s32)r0 > (s32)0x0) {
            do {
                r0 = *(u32*)((u8*)r5 + 0xC);
                r3 = r0 + r4;
                r0 = *(u8*)((u8*)r3 + 0x4);
                if ((u32)r0 == (u32)0x0) continue;
                r0 = *(u16*)((u8*)r3 + 0x6);
                if ((u32)r0 != (u32)r30) continue;
            } while (--ctr != 0);
        }
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((u32)r31 == (u32)0x0) { r3 = 0x1; return; }
        fn_800F0308();
    }

    r3 = 0x1;

    return;
}

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
void fn_800F8268(void) {
    extern u8 lbl_80401C10[];
    extern u8 lbl_8047AC48[];
    extern u8 lbl_8047AC4C[];
    extern void fn_800AB150();
    extern void fn_800D0F44();
    extern void fn_800F8428();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)sp + 0x8;
    fn_800AB150();
    r3 = (u32)lbl_80401C10;
    r30 = (u32)sp + 0x8;
    r31 = (u32)lbl_80401C10;
    for (r29 = 0x0; (s32)r29 < (s32)0x4; r29 = r29 + 0x1, r30 = r30 + 0xc) {
        r0 = *(u32*)((u8*)r31 + 0x0);
        r4 = r29 + 0x1;
        r3 = 0x0;
        if ((s32)r0 == (s32)r4) {
            r28 = r31;
        } else {
            r28 = r31 + 0x6c;
            r0 = *(u32*)((u8*)r31 + 0x6C);
            if ((s32)r0 == (s32)r4) {
                /* found */
            } else if ((s32)*(u32*)((u8*)r28 + 0x6C) == (s32)r4) {
                /* found */
            } else if ((s32)*(u32*)((u8*)r28 + 0x6C) == (s32)r4) {
                /* found */
            } else {
                r28 = r3;
            }
        }
        if ((u32)r28 == (u32)0x0) continue;
        r0 = *(u8*)((u8*)r30 + 0xA);
        r0 = (s8)r0;
        if ((s32)r0 == (s32)-0x1) {
            r3 = r29;
            fn_800D0F44();
            if ((u32)r3 == (u32)0x8) {
                r0 = 0x3;
                *(u32*)((u8*)r28 + 0xC) = r0;
            } else if ((u32)r3 == (u32)0x40) {
                r0 = 0x4;
                *(u32*)((u8*)r28 + 0xC) = r0;
            }
            r3 = r28 + 0x18;
            r4 = 0x0;
            r5 = 0xc;
            memset((void*)r3, (int)r4, (u32)r5);
            r0 = (0x8000 << 16);
            r3 = *(u32*)lbl_8047AC4C;
            r0 = (u32)r0 >> r29;
            r0 = r3 | r0;
            *(u32*)lbl_8047AC4C = r0;
        } else if ((s32)r0 == (s32)0x0) {
            r0 = *(u32*)((u8*)r28 + 0xC);
            if ((s32)r0 == (s32)0x3) {
                r3 = r29;
                fn_800D0F44();
                r0 = (0x900 << 16);
                if ((s32)r3 == (s32)r0) {
                    r0 = 0x0;
                    *(u32*)((u8*)r28 + 0x4) = r0;
                } else {
                    r0 = 0x2;
                    *(u32*)((u8*)r28 + 0x4) = r0;
                }
                r0 = 0x0;
                *(u32*)((u8*)r28 + 0xC) = r0;
            }
            r0 = *(u8*)((u8*)r30 + 0x3);
            r4 = r30;
            r3 = r28 + 0x18;
            r5 = 0xc;
            r0 = -r0;
            *(u8*)((u8*)r30 + 0x3) = r0;
            r0 = *(u8*)((u8*)r30 + 0x5);
            r0 = -r0;
            *(u8*)((u8*)r30 + 0x5) = r0;
            memcpy((void*)r3, (const void*)r4, (u32)r5);
            r0 = (0x8000 << 16);
            r3 = *(u32*)lbl_8047AC4C;
            r0 = (u32)r0 >> r29;
            r0 = r3 & ~r0;
            *(u32*)lbl_8047AC4C = r0;
        }
        /* else: out of range, skip */
    }
    fn_800F8428();
    r3 = *(u32*)lbl_8047AC48;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047AC48 = r0;
    return;
}

/* 0x800F8428 | 0x22C */
void fn_800F8428(void) {
    extern u8 lbl_80401C10[];
    extern void fn_800AB4FC();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = (u32)lbl_80401C10;
    r4 = 0x4;
    r3 = (u32)lbl_80401C10;
    r0 = 0x0;
    r7 = 0x0;
    r5 = r3 + 0x1c0;
    r6 = r3 + 0x1b0;
    ctr_fn = (void(*)(void))r4;
    do {
        r4 = r3 + 0x0;
        r10 = r7 + 0x1;
        r8 = *(u32*)((u8*)r4 + 0x0);
        r9 = 0x0;
        if ((s32)r8 == (s32)r10) {

        } else {
            r8 = *(u32*)((u8*)r4 + 0x6C);
            if ((s32)r8 == (s32)r10) {

            } else {
                r8 = *(u32*)((u8*)r4 + 0x6C);
                if ((s32)r8 == (s32)r10) {

                } else {
                    r8 = *(u32*)((u8*)r4 + 0x6C);
                    if ((s32)r8 == (s32)r10) {

                    } else {
                        r4 = r9;
                    }
                    }
                    }
                    }

        if ((u32)r4 != (u32)0x0) {
            r8 = *(u32*)((u8*)r4 + 0xC);
            if ((s32)r8 == (s32)0x0) {
                r8 = *(u32*)((u8*)r4 + 0x5C);
                if ((s32)r8 != (s32)0x2) {
                    if ((s32)r8 < (s32)0x2) {
                        if ((s32)r8 < (s32)0x1) {

                        } else {
                            if ((s32)r8 < (s32)0x4) {
                        r8 = *(u32*)((u8*)r5 + 0x0);
                        if ((u32)r8 != (u32)0x2) {
                            r8 = 0x2;
                            r0 = 0x1;
                            *(u32*)((u8*)r5 + 0x0) = r8;
                                }
                        }
                        }
                        }
                        }

                r8 = *(u32*)((u8*)r4 + 0x64);
                if ((u32)r8 != (u32)0x0) {
                    r8 = *(u32*)((u8*)r4 + 0x64);
                    /* subic. r8, r8, 0x1 */;
                    *(u32*)((u8*)r4 + 0x64) = r8;
                    if ((u32)r8 == (u32)0x0) {
                        r8 = 0x2;
                        *(u32*)((u8*)r4 + 0x5C) = r8;
                }
                }
                r8 = *(u8*)((u8*)r4 + 0x68);
                if ((u32)r8 != (u32)0x0) {
                    r9 = *(u32*)((u8*)r4 + 0x60);
                    r8 = *(u8*)((u8*)r4 + 0x68);
                    if ((u32)r9 < (u32)r8) {
                        r9 = 0x2;
                        r8 = 0x0;
                        *(u32*)((u8*)r4 + 0x5C) = r9;
                        *(u8*)((u8*)r4 + 0x68) = r8;

                    } else {
                        r9 = *(u8*)((u8*)r4 + 0x68);
                        r8 = *(u32*)((u8*)r4 + 0x60);
                        r8 = r8 - r9;
                        *(u32*)((u8*)r4 + 0x60) = r8;
                    }
                    }
                    }
                    }

        r5 = r5 + 0x4;
        r6 = r6 + 0x4;
        r7 = r7 + 0x1;
    } while (--ctr != 0);
    r0 = r0 & 0xFF;
    if ((u32)r9 != (u32)r8) {
        r3 = r3 + 0x1c0;
        fn_800AB4FC();
    }
    return;
}

/* 0x800F8654 | 0x400 */
void fn_800F8654(void) {
    extern u8 lbl_8047CCC8[];
    extern u8 lbl_8047CCD0[];
    extern u8 lbl_8047CCD4[];
    extern u8 lbl_8047CCD8[];
    extern u8 lbl_8047CCDC[];
    extern u8 lbl_8047CCE0[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;

    r12 = (s8)r4;
    /* subi r0, r12, 0x2 */;
    r30 = *(u8*)((u8*)r6 + 0x0);
    r30 = (s8)r30;
    if ((s32)r30 >= (s32)r0) {
        r0 = r12 + 0x2;
        if ((s32)r30 <= (s32)r0) {
            r30 = *(u8*)((u8*)r7 + 0x0);
            r12 = (s8)r5;
            /* subi r0, r12, 0x2 */;
            r30 = (s8)r30;
            if ((s32)r30 >= (s32)r0) {
                r0 = r12 + 0x2;
                if ((s32)r30 > (s32)r0) {
                }
                }
                }
        r0 = *(u32*)((u8*)r3 + 0x14);
        if ((s32)r0 == (s32)0x0) {
            r0 = (s8)r4;
            r30 = (0x4330 << 16);
            /* xoris r0, r0, 0x8000 */;
            r12 = *(u8*)((u8*)r3 + 0x10);
            *(u32*)(sp + 0xC) = r0;
            r0 = (s8)r5;
            /* xoris r0, r0, 0x8000 */;
            f4 = *(f64*)lbl_8047CCC8;
            f1 = *(f32*)((u8*)r10 + 0x0);
            f0 = *(f64*)(sp + 0x8);
            f3 = f0 - f4;
            f2 = *(f64*)lbl_8047CCE0;
            f0 = *(f64*)(sp + 0x10);
            f1 = f3 - f1;
            *(u32*)(sp + 0x1C) = r0;
            f0 = f0 - f2;
            f1 = f1 / f0;
            f0 = *(f64*)(sp + 0x18);
            *(f32*)((u8*)r8 + 0x0) = f1;
            f1 = f0 - f4;
            r0 = *(u8*)((u8*)r3 + 0x10);
            f0 = *(f32*)((u8*)r11 + 0x0);
            *(u32*)(sp + 0x24) = r0;
            f1 = f1 - f0;
            f0 = *(f64*)(sp + 0x20);
            f0 = f0 - f2;
            f0 = f1 / f0;
            *(f32*)((u8*)r9 + 0x0) = f0;

        } else {
            if ((s32)r0 == (s32)0x1) {
                r30 = (s8)r4;
                r12 = *(u8*)((u8*)r3 + 0x10);
                r31 = (0x4330 << 16);
                r0 = (s8)r5;
                /* xoris r30, r30, 0x8000 */;
                /* xoris r0, r0, 0x8000 */;
                f6 = *(f64*)lbl_8047CCC8;
                f4 = *(f64*)lbl_8047CCE0;
                f0 = *(f64*)(sp + 0x20);
                f2 = f0 - f6;
                f1 = *(f32*)((u8*)r10 + 0x0);
                f3 = *(f32*)lbl_8047CCD4;
                f0 = *(f64*)(sp + 0x18);
                f5 = f2 - f1;
                *(u32*)(sp + 0x14) = r0;
                f0 = f0 - f4;
                f2 = f0 * f3;
                f0 = *(f64*)(sp + 0x10);
                f1 = f0 - f6;
                f0 = f5 / f2;
                *(f32*)((u8*)r8 + 0x0) = f0;
                r0 = *(u8*)((u8*)r3 + 0x10);
                f0 = *(f32*)((u8*)r11 + 0x0);
                *(u32*)(sp + 0xC) = r0;
                f1 = f1 - f0;
                f0 = *(f64*)(sp + 0x8);
                f0 = f0 - f4;
                f0 = f0 * f3;
                f0 = f1 / f0;
                *(f32*)((u8*)r9 + 0x0) = f0;
            }
        }

        r0 = 0x0;
        *(u8*)((u8*)r3 + 0x11) = r0;
                }
    *(u8*)((u8*)r6 + 0x0) = r4;
    *(u8*)((u8*)r7 + 0x0) = r5;
    f1 = *(f32*)((u8*)r10 + 0x0);
    f0 = *(f32*)((u8*)r8 + 0x0);
    f0 = f1 + f0;
    *(f32*)((u8*)r10 + 0x0) = f0;
    f1 = *(f32*)((u8*)r11 + 0x0);
    f0 = *(f32*)((u8*)r9 + 0x0);
    f0 = f1 + f0;
    *(f32*)((u8*)r11 + 0x0) = f0;
    r0 = *(u32*)((u8*)r3 + 0x14);
    if ((s32)r0 == (s32)0x1) {
        r4 = *(u8*)((u8*)r3 + 0x11);
        r0 = r4 + 0x1;
        *(u8*)((u8*)r3 + 0x11) = r0;
        r4 = *(u8*)((u8*)r3 + 0x11);
        r0 = *(u8*)((u8*)r3 + 0x10);
        if ((u32)r4 < (u32)r0) {
            f1 = *(f32*)((u8*)r8 + 0x0);
            f2 = *(f32*)lbl_8047CCD8;
            f0 = *(f32*)lbl_8047CCD0;
            f1 = f1 * f2;
            *(f32*)((u8*)r8 + 0x0) = f1;
            f1 = *(f32*)((u8*)r9 + 0x0);
            f1 = f1 * f2;
            *(f32*)((u8*)r9 + 0x0) = f1;
            f2 = *(f32*)((u8*)r8 + 0x0);
            if (f2 > f0) {
                f1 = f2;
            } else {

                f1 = -f2;
            }
            f0 = *(f32*)lbl_8047CCDC;
            if (f1 < f0) {
                f0 = *(f32*)lbl_8047CCD0;
                if (f2 > f0) {
                    r0 = 0x1;
                } else {

                    r0 = -0x1;
                }
                /* xoris r3, r0, 0x8000 */;
                r0 = (0x4330 << 16);
                f1 = *(f64*)lbl_8047CCC8;
                *(u32*)(sp + 0x20) = r0;
                f0 = *(f64*)(sp + 0x20);
                f0 = f0 - f1;
                *(f32*)((u8*)r8 + 0x0) = f0;
            }
            f1 = *(f32*)((u8*)r9 + 0x0);
            f0 = *(f32*)lbl_8047CCD0;
            if (f1 > f0) {
            } else {

                f1 = -f1;
            }
            f0 = *(f32*)lbl_8047CCDC;
            if (f1 < f0) {
                f1 = *(f32*)((u8*)r8 + 0x0);
                f0 = *(f32*)lbl_8047CCD0;
                if (f1 > f0) {
                    r0 = 0x1;
                } else {

                    r0 = -0x1;
                }
                /* xoris r3, r0, 0x8000 */;
                r0 = (0x4330 << 16);
                f1 = *(f64*)lbl_8047CCC8;
                *(u32*)(sp + 0x20) = r0;
                f0 = *(f64*)(sp + 0x20);
                f0 = f0 - f1;
                *(f32*)((u8*)r9 + 0x0) = f0;
    }
    }
    }
    f1 = *(f32*)((u8*)r8 + 0x0);
    f0 = *(f32*)lbl_8047CCD0;
    if (f1 < f0) {
        r3 = *(u8*)((u8*)r6 + 0x0);
        r0 = (0x4330 << 16);
        *(u32*)(sp + 0x20) = r0;
        r3 = (s8)r3;
        f1 = *(f64*)lbl_8047CCC8;
        /* xoris r3, r3, 0x8000 */;
        f2 = *(f32*)((u8*)r10 + 0x0);
        f0 = *(f64*)(sp + 0x20);
        f0 = f0 - f1;
        if (f2 < f0) {
            *(u32*)(sp + 0x20) = r0;
            f0 = *(f64*)(sp + 0x20);
            f0 = f0 - f1;
            *(f32*)((u8*)r10 + 0x0) = f0;

        } else {
            r3 = *(u8*)((u8*)r6 + 0x0);
            r0 = (0x4330 << 16);
            *(u32*)(sp + 0x20) = r0;
            r3 = (s8)r3;
            f1 = *(f64*)lbl_8047CCC8;
            /* xoris r3, r3, 0x8000 */;
            f2 = *(f32*)((u8*)r10 + 0x0);
            f0 = *(f64*)(sp + 0x20);
            f0 = f0 - f1;
            if (f2 > f0) {
                *(u32*)(sp + 0x20) = r0;
                f0 = *(f64*)(sp + 0x20);
                f0 = f0 - f1;
                *(f32*)((u8*)r10 + 0x0) = f0;
                }
            }
        }

    f1 = *(f32*)((u8*)r9 + 0x0);
    f0 = *(f32*)lbl_8047CCD0;
    if (f1 < f0) {
        r3 = *(u8*)((u8*)r7 + 0x0);
        r0 = (0x4330 << 16);
        *(u32*)(sp + 0x20) = r0;
        r3 = (s8)r3;
        f1 = *(f64*)lbl_8047CCC8;
        /* xoris r3, r3, 0x8000 */;
        f2 = *(f32*)((u8*)r11 + 0x0);
        f0 = *(f64*)(sp + 0x20);
        f0 = f0 - f1;
        if (f2 >= f0) return;
        *(u32*)(sp + 0x20) = r0;
        f0 = *(f64*)(sp + 0x20);
        f0 = f0 - f1;
        *(f32*)((u8*)r11 + 0x0) = f0;
        return;
    }
    r3 = *(u8*)((u8*)r7 + 0x0);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x20) = r0;
    r3 = (s8)r3;
    f1 = *(f64*)lbl_8047CCC8;
    /* xoris r3, r3, 0x8000 */;
    f2 = *(f32*)((u8*)r11 + 0x0);
    f0 = *(f64*)(sp + 0x20);
    f0 = f0 - f1;
    if (f2 <= f0) return;
    *(u32*)(sp + 0x20) = r0;
    f0 = *(f64*)(sp + 0x20);
    f0 = f0 - f1;
    *(f32*)((u8*)r11 + 0x0) = f0;

    return;
}

/* 0x800F8A54 | 0x708 */
void fn_800F8A54(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047CCC8[];
    extern u8 lbl_8047CCD0[];
    extern u8 lbl_8047CCE8[];
    extern u8 lbl_8047CCF0[];
    extern u8 lbl_8047CCF8[];
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((s32)r0 == (s32)0x1) return;
    if ((s32)r0 < (s32)0x1) {
        if ((s32)r0 < (s32)0x0) {
            return;
        }
        if ((s32)r0 >= (s32)0x3) return;
    r6 = *(u8*)((u8*)r3 + 0x26);
    r5 = *(u8*)((u8*)r3 + 0x27);
    r6 = (s8)r6;
    r5 = (s8)r5;
    if ((s32)r6 > (s32)-0xa || (s32)r6 >= (s32)0xa) {

        r6 = 0x0;

    } else if ((s32)r6 > (s32)0x0) {
        /* subi r6, r6, 0xa */;

    } else {
        r6 = r6 + 0xa;
    }
    if ((s32)r5 > (s32)-0xa || (s32)r5 >= (s32)0xa) {

        r5 = 0x0;

    } else if ((s32)r5 > (s32)0x0) {
        /* subi r5, r5, 0xa */;

    } else {
        r5 = r5 + 0xa;
    }
    r4 = r6 * r6;
    r0 = r5 * r5;
    r0 = r4 + r0;
    if ((s32)r0 > (s32)0xc40) {
        /* xoris r4, r0, 0x8000 */;
        r0 = (0x4330 << 16);
        f2 = *(f64*)lbl_8047CCC8;
        *(u32*)(sp + 0x30) = r0;
        f0 = *(f32*)lbl_8047CCD0;
        f1 = *(f64*)(sp + 0x30);
        f4 = f1 - f2;
        if (f4 > f0) {
            /* frsqrte f1, f4 */;
            f3 = *(f64*)lbl_8047CCE8;
            f2 = *(f64*)lbl_8047CCF0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f0 = f1 * f0;
            f4 = f4 * f0;
            f4 = (f32)f4;

        } else {
            f0 = *(f64*)lbl_8047CCF8;
            if (f4 < f0) {
                r4 = (u32)lbl_80478AC0;
                f4 = *(f32*)lbl_80478AC0;

            } else {
                *(f32*)(sp + 0x14) = f4;
                r0 = (0x7f80 << 16);
                r4 = r7 & 0x7F800000;
                if ((s32)r4 != (s32)r0) {
                    if ((s32)r4 < (s32)r0) {
                        if ((s32)r4 != (s32)0x0) {
                r0 = 0x4;
                if ((s32)r0 == (s32)0x1) {
                    r4 = (u32)lbl_80478AC0;
                    f4 = *(f32*)lbl_80478AC0;
                }
            }
            }

        f0 = (f64)(s32)f4;
        r4 = r6 * 0x38;
        *(f64*)(sp + 0x30) = f0;
        r0 = r5 * 0x38;
        r6 = (s32)r4 / (s32)r5;
        r5 = (s32)r0 / (s32)r5;
    }
    *(u8*)((u8*)r3 + 0x26) = r6;
    *(u8*)((u8*)r3 + 0x27) = r5;
    r6 = *(u8*)((u8*)r3 + 0x28);
    r5 = *(u8*)((u8*)r3 + 0x29);
    r6 = (s8)r6;
    r5 = (s8)r5;
    if ((s32)r6 > (s32)-0xa || (s32)r6 >= (s32)0xa) {

        r6 = 0x0;

    } else if ((s32)r6 > (s32)0x0) {
        /* subi r6, r6, 0xa */;

    } else {
        r6 = r6 + 0xa;
    }
    if ((s32)r5 > (s32)-0xa || (s32)r5 >= (s32)0xa) {

        r5 = 0x0;

    } else if ((s32)r5 > (s32)0x0) {
        /* subi r5, r5, 0xa */;

    } else {
        r5 = r5 + 0xa;
    }
    r4 = r6 * r6;
    r0 = r5 * r5;
    r0 = r4 + r0;
    if ((s32)r0 > (s32)0x790) {
        /* xoris r4, r0, 0x8000 */;
        r0 = (0x4330 << 16);
        f2 = *(f64*)lbl_8047CCC8;
        *(u32*)(sp + 0x30) = r0;
        f0 = *(f32*)lbl_8047CCD0;
        f1 = *(f64*)(sp + 0x30);
        f4 = f1 - f2;
        if (f4 > f0) {
            /* frsqrte f1, f4 */;
            f3 = *(f64*)lbl_8047CCE8;
            f2 = *(f64*)lbl_8047CCF0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f0 = f1 * f0;
            f4 = f4 * f0;
            f4 = (f32)f4;

        } else {
            f0 = *(f64*)lbl_8047CCF8;
            if (f4 < f0) {
                r4 = (u32)lbl_80478AC0;
                f4 = *(f32*)lbl_80478AC0;

            } else {
                *(f32*)(sp + 0x10) = f4;
                r0 = (0x7f80 << 16);
                r4 = r7 & 0x7F800000;
                if ((s32)r4 != (s32)r0) {
                    if ((s32)r4 < (s32)r0) {
                        if ((s32)r4 != (s32)0x0) {
                r0 = 0x4;
                if ((s32)r0 == (s32)0x1) {
                    r4 = (u32)lbl_80478AC0;
                    f4 = *(f32*)lbl_80478AC0;
                }
            }
            }

        f0 = (f64)(s32)f4;
        r4 = r6 * 0x2c;
        *(f64*)(sp + 0x30) = f0;
        r0 = r5 * 0x2c;
        r6 = (s32)r4 / (s32)r5;
        r5 = (s32)r0 / (s32)r5;
    }
    *(u8*)((u8*)r3 + 0x28) = r6;
    *(u8*)((u8*)r3 + 0x29) = r5;
    f1 = *(f32*)((u8*)r3 + 0x48);
    f0 = *(f32*)((u8*)r3 + 0x4C);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f1;
    *(f64*)(sp + 0x28) = f0;
    r6 = (s8)r0;
    r5 = (s8)r0;
    if ((s32)r6 > (s32)-0xa || (s32)r6 >= (s32)0xa) {

        r6 = 0x0;

    } else if ((s32)r6 > (s32)0x0) {
        /* subi r6, r6, 0xa */;

    } else {
        r6 = r6 + 0xa;
    }
    if ((s32)r5 > (s32)-0xa || (s32)r5 >= (s32)0xa) {

        r5 = 0x0;

    } else if ((s32)r5 > (s32)0x0) {
        /* subi r5, r5, 0xa */;

    } else {
        r5 = r5 + 0xa;
    }
    r4 = r6 * r6;
    r0 = r5 * r5;
    r0 = r4 + r0;
    if ((s32)r0 > (s32)0xc40) {
        /* xoris r4, r0, 0x8000 */;
        r0 = (0x4330 << 16);
        f2 = *(f64*)lbl_8047CCC8;
        *(u32*)(sp + 0x30) = r0;
        f0 = *(f32*)lbl_8047CCD0;
        f1 = *(f64*)(sp + 0x30);
        f4 = f1 - f2;
        if (f4 > f0) {
            /* frsqrte f1, f4 */;
            f3 = *(f64*)lbl_8047CCE8;
            f2 = *(f64*)lbl_8047CCF0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f0 = f1 * f0;
            f4 = f4 * f0;
            f4 = (f32)f4;

        } else {
            f0 = *(f64*)lbl_8047CCF8;
            if (f4 < f0) {
                r4 = (u32)lbl_80478AC0;
                f4 = *(f32*)lbl_80478AC0;

            } else {
                *(f32*)(sp + 0xC) = f4;
                r0 = (0x7f80 << 16);
                r4 = r7 & 0x7F800000;
                if ((s32)r4 != (s32)r0) {
                    if ((s32)r4 < (s32)r0) {
                        if ((s32)r4 != (s32)0x0) {
                r0 = 0x4;
                if ((s32)r0 == (s32)0x1) {
                    r4 = (u32)lbl_80478AC0;
                    f4 = *(f32*)lbl_80478AC0;
                }
            }
            }

        f0 = (f64)(s32)f4;
        r4 = r6 * 0x38;
        *(f64*)(sp + 0x30) = f0;
        r0 = r5 * 0x38;
        r6 = (s32)r4 / (s32)r5;
        r5 = (s32)r0 / (s32)r5;
    }
    *(u8*)((u8*)r3 + 0x58) = r6;
    *(u8*)((u8*)r3 + 0x59) = r5;
    f1 = *(f32*)((u8*)r3 + 0x50);
    f0 = *(f32*)((u8*)r3 + 0x54);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f1;
    *(f64*)(sp + 0x28) = f0;
    r6 = (s8)r0;
    r5 = (s8)r0;
    if ((s32)r6 > (s32)-0xa || (s32)r6 >= (s32)0xa) {

        r6 = 0x0;

    } else if ((s32)r6 > (s32)0x0) {
        /* subi r6, r6, 0xa */;

    } else {
        r6 = r6 + 0xa;
    }
    if ((s32)r5 > (s32)-0xa || (s32)r5 >= (s32)0xa) {

        r5 = 0x0;

    } else if ((s32)r5 > (s32)0x0) {
        /* subi r5, r5, 0xa */;

    } else {
        r5 = r5 + 0xa;
    }
    r4 = r6 * r6;
    r0 = r5 * r5;
    r0 = r4 + r0;
    if ((s32)r0 > (s32)0x790) {
        /* xoris r4, r0, 0x8000 */;
        r0 = (0x4330 << 16);
        f2 = *(f64*)lbl_8047CCC8;
        *(u32*)(sp + 0x30) = r0;
        f0 = *(f32*)lbl_8047CCD0;
        f1 = *(f64*)(sp + 0x30);
        f4 = f1 - f2;
        if (f4 > f0) {
            /* frsqrte f1, f4 */;
            f3 = *(f64*)lbl_8047CCE8;
            f2 = *(f64*)lbl_8047CCF0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f0 = f1 * f0;
            f4 = f4 * f0;
            f4 = (f32)f4;

        } else {
            f0 = *(f64*)lbl_8047CCF8;
            if (f4 < f0) {
                r4 = (u32)lbl_80478AC0;
                f4 = *(f32*)lbl_80478AC0;

            } else {
                *(f32*)(sp + 0x8) = f4;
                r0 = (0x7f80 << 16);
                r4 = r7 & 0x7F800000;
                if ((s32)r4 != (s32)r0) {
                    if ((s32)r4 < (s32)r0) {
                        if ((s32)r4 != (s32)0x0) {
                r0 = 0x4;
                if ((s32)r0 == (s32)0x1) {
                    r4 = (u32)lbl_80478AC0;
                    f4 = *(f32*)lbl_80478AC0;
                }
            }
            }

        f0 = (f64)(s32)f4;
        r4 = r6 * 0x2c;
        *(f64*)(sp + 0x30) = f0;
        r0 = r5 * 0x2c;
        r6 = (s32)r4 / (s32)r5;
        r5 = (s32)r0 / (s32)r5;
    }
    *(u8*)((u8*)r3 + 0x5A) = r6;
    *(u8*)((u8*)r3 + 0x5B) = r5;

    return;
}
}
}
}
}
}
}
}
}
}
}
}
}
}

/* 0x800F915C | 0xB4 */
void fn_800F915C(void) {
    extern u8 lbl_8047AC5C[];
    extern u8 lbl_8047AC60[];
    extern void fn_800E209C();
    extern void fn_800E24B0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r29 = r3;
    r30 = *(u32*)lbl_8047AC5C;
    r31 = *(u32*)lbl_8047AC60;
    while ((u32)r31 != (u32)0x0) {
            /* subi r31, r31, 0x1 */;
            r3 = *(u32*)((u8*)r30 + 0x4);
            if ((u32)r3 != (u32)0x0) {
                r4 = *(u32*)((u8*)r30 + 0x8);
                if ((u32)r4 == (u32)r29) {
                    r12 = *(u32*)((u8*)r30 + 0x10);
                    if ((u32)r12 != (u32)0x0) {
                        r5 = *(u32*)((u8*)r30 + 0xC);
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        r0 = r3 & 0xFF;
                        if ((u32)r12 != (u32)0x0) {
                        }
                        r3 = *(u16*)((u8*)r30 + 0x0);
                        if ((u32)r3 != (u32)0x0) {
                            fn_800E24B0();
                            r3 = *(u16*)((u8*)r30 + 0x0);
                            fn_800E209C();
                            r0 = 0x0;
                            *(u16*)((u8*)r30 + 0x0) = r0;
                        }
                        r0 = 0x0;
                        *(u32*)((u8*)r30 + 0x4) = r0;
                    }
                    }
                        }
            r30 = r30 + 0x14;
    }
    return;
}

/* 0x800F9210 | 0xC4 */
void fn_800F9210(void) {
    extern u8 lbl_8047AC5C[];
    extern u8 lbl_8047AC60[];
    extern void fn_800E209C();
    extern void fn_800E24B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = *(u32*)lbl_8047AC60;
    r31 = *(u32*)lbl_8047AC5C;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 != (u32)0x0) {
        do {
            r0 = *(u32*)((u8*)r31 + 0x4);
            if ((u32)r0 == (u32)0x0) continue;
            r0 = *(u32*)((u8*)r31 + 0x8);
            if ((u32)r0 != (u32)r3) continue;
            r0 = *(u32*)((u8*)r31 + 0xC);
            if ((u32)r0 != (u32)r4) continue;
        } while (--ctr != 0);
    }
    if ((u32)r31 == (u32)0x0) return;
    r12 = *(u32*)((u8*)r31 + 0x10);
    if ((u32)r12 != (u32)0x0) {
        r3 = *(u32*)((u8*)r31 + 0x4);
        r4 = *(u32*)((u8*)r31 + 0x8);
        r5 = *(u32*)((u8*)r31 + 0xC);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        r0 = r3 & 0xFF;
        if ((u32)r12 == (u32)0x0) return;
    }
    r3 = *(u16*)((u8*)r31 + 0x0);
    if ((u32)r3 != (u32)0x0) {
        fn_800E24B0();
        r3 = *(u16*)((u8*)r31 + 0x0);
        fn_800E209C();
        r0 = 0x0;
        *(u16*)((u8*)r31 + 0x0) = r0;
    }
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x4) = r0;

    return;
}

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
void fn_800F9378(void) {
    extern u8 lbl_8047AC5C[];
    extern u8 lbl_8047AC60[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r8 = *(u32*)lbl_8047AC5C;
    r9 = *(u32*)lbl_8047AC60;
    r7 = r8;
    /* Search for existing entry matching r4/r5 */
    r7 = 0x0; /* default: not found */
    for (ctr = (u32)r9; ctr != 0; ctr--) {
        u32 base = (u32)r8 + ((u32)r9 - ctr) * 0x14;
        r0 = *(u32*)((u8*)base + 0x4);
        if ((u32)r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)base + 0x8);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u32*)((u8*)base + 0xC);
                if ((u32)r0 == (u32)r5) {
                    r7 = base;
                    break;
                }
            }
        }
    }
    if (ctr == 0) r7 = 0x0;
    if ((u32)r7 != (u32)0x0) return;
    /* Search for free slot */
    r8 = 0x0; /* default: not found */
    for (ctr = (u32)r9; ctr != 0; ctr--) {
        u32 base2 = *(u32*)lbl_8047AC5C + ((u32)r9 - ctr) * 0x14;
        r0 = *(u32*)((u8*)base2 + 0x4);
        if ((u32)r0 == (u32)0x0) {
            r8 = base2;
            break;
        }
    }
    if (ctr == 0) r8 = 0x0;
    if ((u32)r8 == (u32)0x0) return;
    r0 = 0x0;
    *(u16*)((u8*)r8 + 0x0) = r0;
    *(u32*)((u8*)r8 + 0x4) = r3;
    *(u32*)((u8*)r8 + 0x8) = r4;
    *(u32*)((u8*)r8 + 0xC) = r5;
    *(u32*)((u8*)r8 + 0x10) = r6;
    return;
}

/* 0x800F9418 | 0x12C */
void fn_800F9418(void) {
    extern u8 lbl_8047AC5C[];
    extern u8 lbl_8047AC60[];
    extern void fn_800E209C();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r7;
    r29 = r6;
    r28 = r5;
    r31 = *(u32*)lbl_8047AC5C;
    r8 = *(u32*)lbl_8047AC60;
    r5 = r31;
    /* Search for existing entry matching r28/r29 */
    r5 = 0x0;
    for (ctr = (u32)r8; ctr != 0; ctr--) {
        r0 = *(u32*)((u8*)r5 + 0x4);
        if ((u32)r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)r5 + 0x8);
            if ((u32)r0 == (u32)r28) {
                r0 = *(u32*)((u8*)r5 + 0xC);
                if ((u32)r0 == (u32)r29) break;
            }
        }
        r5 = r5 + 0x14;
    }
    if (ctr == 0) r5 = 0x0;
    if ((u32)r5 != (u32)0x0) {
        r3 = 0x0;
        return;
    }
    /* Search for free slot */
    for (ctr = (u32)r8; ctr != 0; ctr--) {
        r0 = *(u32*)((u8*)r31 + 0x4);
        if ((u32)r0 == (u32)0x0) break;
        r31 = r31 + 0x14;
    }
    if (ctr == 0) r31 = 0x0;
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_800E2C04();
    *(u16*)((u8*)r31 + 0x0) = r3;
    r3 = *(u16*)((u8*)r31 + 0x0);
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_800E27B0();
    *(u32*)((u8*)r31 + 0x4) = r3;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r0 == (u32)0x0) {
        r3 = *(u16*)((u8*)r31 + 0x0);
        fn_800E209C();
        r3 = 0x0;
        return;
    }
    *(u32*)((u8*)r31 + 0x8) = r28;
    *(u32*)((u8*)r31 + 0xC) = r29;
    *(u32*)((u8*)r31 + 0x10) = r30;
    r3 = *(u32*)((u8*)r31 + 0x4);

    return;
}

/* 0x800F9544 | 0x12C */
void fn_800F9544(void) {
    extern u8 lbl_8047AC5C[];
    extern u8 lbl_8047AC60[];
    extern void fn_800E209C();
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r6;
    r29 = r5;
    r28 = r4;
    r31 = *(u32*)lbl_8047AC5C;
    r7 = *(u32*)lbl_8047AC60;
    r4 = r31;
    /* Search for existing entry matching r28/r29 */
    r4 = 0x0;
    for (ctr = (u32)r7; ctr != 0; ctr--) {
        r0 = *(u32*)((u8*)r4 + 0x4);
        if ((u32)r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)r4 + 0x8);
            if ((u32)r0 == (u32)r28) {
                r0 = *(u32*)((u8*)r4 + 0xC);
                if ((u32)r0 == (u32)r29) break;
            }
        }
        r4 = r4 + 0x14;
    }
    if (ctr == 0) r4 = 0x0;
    if ((u32)r4 != (u32)0x0) {
        r3 = 0x0;
        return;
    }
    /* Search for free slot */
    for (ctr = (u32)r7; ctr != 0; ctr--) {
        r0 = *(u32*)((u8*)r31 + 0x4);
        if ((u32)r0 == (u32)0x0) break;
        r31 = r31 + 0x14;
    }
    if (ctr == 0) r31 = 0x0;
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_800E3534();
    *(u16*)((u8*)r31 + 0x0) = r3;
    r3 = *(u16*)((u8*)r31 + 0x0);
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_800E27B0();
    *(u32*)((u8*)r31 + 0x4) = r3;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r0 == (u32)0x0) {
        r3 = *(u16*)((u8*)r31 + 0x0);
        fn_800E209C();
        r3 = 0x0;
        return;
    }
    *(u32*)((u8*)r31 + 0x8) = r28;
    *(u32*)((u8*)r31 + 0xC) = r29;
    *(u32*)((u8*)r31 + 0x10) = r30;
    r3 = *(u32*)((u8*)r31 + 0x4);

    return;
}

/* 0x74 | fn_800F9670 | generic */
void fn_800F9670(void) {
    /* refs: lbl_8047AC58, lbl_8047AC5C, lbl_8047AC60 */
    fn_800E3534();
    fn_800E27B0();
}

/* 0x800F96E4 | 0x408 */
void fn_800F96E4(void) {
    extern u8 lbl_80271700[];
    extern u8 lbl_80315678[];
    extern u8 lbl_80401DE0[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_80478B08;
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r22 = r5;
    r23 = 0x0;
    if ((u32)r22 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r4 <= (s32)0x0) {
        r3 = 0x0;
        return;
    }
    /* subi r0, r4, 0x1 */;
    r28 = r31;
    r24 = r0 << 1;
    if ((u32)r22 == (u32)0x0) {
        r25 = 0x0;

    } else {
        r3 = *(u32*)&lbl_80478B08;
        r4 = (u32)r22 >> 20;
        r5 = r22 & 0xFFFFF;
        r9 = *(u32*)((u8*)r3 + 0x8);
        while ((u32)r9 != (u32)0x0) {
                r0 = *(u16*)((u8*)r9 + 0x0);
                if ((u32)r0 == (u32)r4) {
                    r6 = *(u16*)((u8*)r9 + 0x4);
                    r8 = r9 + 0x10;
                    r7 = 0x0;
                    goto L_800F97C4;
                    do {
                        r0 = r7 + r6;
                        r3 = (u32)r0 >> 1;
                        r10 = r3 << 3;
                        r0 = *(u32*)(r8 + r10);
                        if ((u32)r0 == (u32)r5) {
                            r0 = (u32)sp + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                            }
                            r3 = r8 + r10;
                            r0 = *(u32*)((u8*)r3 + 0x4);
                            r25 = r9 + r0;
                            goto L_800F97DC;
                        }
                        if ((u32)r0 < (u32)0x0) {
                            r7 = r3 + 0x1;

                        } else {
                            r6 = r3;
                        }
                        L_800F97C4: ;
                    } while ((u32)r7 < (u32)r6);
                }
                r9 = *(u32*)((u8*)r9 + 0x8);
        }
        r25 = 0x0;
    }
    L_800F97DC: ;
    if ((u32)r25 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = (u32)lbl_80401DE0;
    r4 = 0x0;
    r3 = (u32)lbl_80401DE0;
    r5 = 0x68;
    r29 = r3;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = (u32)lbl_80401DE0;
    f0 = *(f32*)lbl_8047CD08;
    r4 = (u32)lbl_80401DE0;
    r5 = 0x1;
    r0 = -0x1;
    *(u8*)((u8*)r4 + 0x0) = r5;
    *(f32*)((u8*)r4 + 0x60) = f0;
    *(f32*)((u8*)r4 + 0x64) = f0;
    *(u32*)((u8*)r4 + 0x24) = r0;
    *(u32*)((u8*)r4 + 0x28) = r25;
    *(u32*)((u8*)r4 + 0x2C) = r25;
    *(u32*)((u8*)r4 + 0x30) = r25;
    r0 = *(u8*)((u8*)r3 + 0x3);
    *(u16*)((u8*)r4 + 0x20) = r0;
    *(u32*)((u8*)r4 + 0x1C) = r22;
    *(u8*)((u8*)r4 + 0x1) = r5;
    L_800F9848: ;
    r27 = r28;
    r22 = r23;
    while (1) {
        r3 = *(u32*)((u8*)r29 + 0x30);
        r4 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r4 == (u32)0x0) {
            r3 = *(u8*)((u8*)r29 + 0x40);
            r0 = (s8)r3;
            if ((s32)r0 == (s32)0x0) {
    r0 = r4 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) {
        r23 = r23 + 0x2;
        if ((u32)r24 >= (u32)r23) {
            *(u16*)((u8*)r28 + 0x0) = r4;
            r28 = r28 + 0x2;
            if ((u32)r0 != (u32)0xffff) goto L_800F9848;
            r3 = *(u32*)((u8*)r29 + 0x30);
            r23 = r23 + 0x1;
            r0 = r3 + 0x1;
            *(u32*)((u8*)r29 + 0x30) = r0;
            r4 = *(u8*)((u8*)r3 + 0x0);
            if ((u32)r24 >= (u32)r23) {
                *(u8*)((u8*)r28 + 0x0) = r4;
                r28 = r28 + 0x1;
            r3 = *(u32*)&lbl_80478B08;
            r26 = *(u32*)((u8*)r29 + 0x30);
            r3 = *(u32*)((u8*)r3 + 0x28);
            r25 = *(u8*)((u8*)r29 + 0x40);
            if ((u32)r3 != (u32)0x0) {
                r0 = *(u8*)((u8*)r29 + 0x1);
                if ((u32)r0 == (u32)0x0) {
                    r0 = r4 << 3;
                    r0 = *(u8*)(r3 + r0);
                    /* extrwi r0, r0, 1, 27 */;
                } else {

                    r0 = r4 << 3;
                    r0 = *(u8*)(r3 + r0);
                    /* extrwi r0, r0, 1, 28 */;
                }
                if ((u32)r0 != (u32)0x0) {
                    r0 = r4 << 3;
                    r21 = r3 + r0;
                    r12 = *(u32*)((u8*)r21 + 0x4);
                    if ((u32)r12 != (u32)0x0) {
                        r3 = r29;
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        r0 = *(u8*)((u8*)r21 + 0x0);
                        /* extrwi r0, r0, 2, 24 */;
                        if ((u32)r0 != (u32)0x0 || (u32)r3 == (u32)0x0) {

                            if ((s32)r0 != (s32)0x2) {
                                if ((s32)r0 < (s32)0x2) {
                                    if ((s32)r0 < (s32)0x1) {

                                    } else {
                                        r30 = r3;

                                    }
                                } else {
                                        if ((u32)r3 == (u32)0x0) {
                                            r30 = 0x0;

                                        } else {
                                            r4 = *(u32*)&lbl_80478B08;
                                            r5 = (u32)r3 >> 20;
                                            r3 = r3 & 0xFFFFF;
                                            r9 = *(u32*)((u8*)r4 + 0x8);
                                            while ((u32)r9 != (u32)0x0) {
                                                    r0 = *(u16*)((u8*)r9 + 0x0);
                                                    if ((u32)r0 == (u32)r5) {
                                                        r6 = *(u16*)((u8*)r9 + 0x4);
                                                        r8 = r9 + 0x10;
                                                        r7 = 0x0;
                                                        goto L_800F9A0C;
                                                        do {
                                                            r0 = r7 + r6;
                                                            r4 = (u32)r0 >> 1;
                                                            r0 = r4 << 3;
                                                            r10 = r8 + r0;
                                                            r0 = *(u32*)((u8*)r10 + 0x0);
                                                            if ((u32)r0 == (u32)r3) {
                                                                r0 = *(u32*)((u8*)r10 + 0x4);
                                                                r30 = r9 + r0;
                                                                goto L_800F9A24;
                                                            }
                                                            if ((u32)r0 < (u32)r3) {
                                                                r7 = r4 + 0x1;

                                                            } else {
                                                                r6 = r4;
                                                            }
                                                            L_800F9A0C: ;
                                                        } while ((u32)r7 < (u32)r6);
                                                    }
                                                    r9 = *(u32*)((u8*)r9 + 0x8);
                                            }
                                            r30 = 0x0;
                                            }
                                        }
                                        }
                            L_800F9A24: ;
                            r4 = *(u8*)((u8*)r29 + 0x40);
                            r3 = (s8)r4;
                            if ((s32)r3 >= (s32)0x3) {
                                r3 = (u32)lbl_80271700;
                                r4 = (u32)lbl_80315678;
                                r3 = (u32)lbl_80271700;
                                r4 = (u32)lbl_80315678;
                                /* crclr cr1eq */;
                                ((void(*)(void))fn_800DD970)();

                            } else {
                                r0 = r4 + 0x1;
                                r3 = r3 << 2;
                                *(u8*)((u8*)r29 + 0x40) = r0;
                                r0 = r3 + 0x34;
                                r3 = *(u32*)((u8*)r29 + 0x30);
                                *(u32*)(r29 + r0) = r3;
                                *(u32*)((u8*)r29 + 0x30) = r30;
                            }
                            }
                            }
                            }
                            }

            r0 = *(u8*)((u8*)r29 + 0x40);
            r3 = (s8)r25;
            r0 = (s8)r0;
            if ((s32)r3 == (s32)r0) {
                r0 = *(u32*)((u8*)r29 + 0x30);
                r0 = r0 - r26;
                r23 = r23 + r0;
                if ((u32)r24 >= (u32)r23) {
                    while (1) {
                        r0 = *(u32*)((u8*)r29 + 0x30);
                        if ((u32)r0 <= (u32)r26) break;
                        r0 = *(u8*)((u8*)r26 + 0x0);
                        r26 = r26 + 0x1;
                        *(u8*)((u8*)r28 + 0x0) = r0;
                        r28 = r28 + 0x1;

                    }
                    goto L_800F9848;
                }
                r28 = r27;

            } else {
                r28 = r27;
                r23 = r22;
                goto L_800F9848;
            }
            }
        }
    r0 = 0x0;
    r3 = r31;
    *(u16*)((u8*)r28 + 0x0) = r0;

    return;
}
}
}
}
}

/* 0x800F9AEC | 0x118 */
void fn_800F9AEC(void) {
    extern u8 lbl_80271300[];
    extern u8 lbl_80271500[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    if ((s32)r5 == (s32)0x1) {
        r7 = 0x0;
        if ((u32)r4 == (u32)0x0) {
            r3 = r7;
            return;
        }
        r5 = (u32)lbl_80271300;
        r9 = *(u16*)((u8*)r4 + 0x0);
        while ((u32)r9 != (u32)0x0) {
            r8 = r5;
            for (r6 = 0x0; (s32)r6 < (s32)0x100; r6 = r6 + 0x1, r8 = r8 + 0x2) {
                r0 = *(u16*)((u8*)r8 + 0x0);
                if ((u32)r9 == (u32)r0) break;
            }
            if ((s32)r6 >= (s32)0x100) {
                r6 = 0xb7;
            }
            if ((u32)r3 != (u32)0x0) {
                r0 = r6 & 0xFF;
                *(u8*)((u8*)r3 + 0x0) = r0;
                r3 = r3 + 0x1;
            }
            r7 = r7 + 0x1;
            r4 = r4 + 0x2;
            r9 = *(u16*)((u8*)r4 + 0x0);
        }
        r3 = r7;
        return;
    }
    /* case 7 and default */
    r7 = 0x0;
    if ((u32)r4 == (u32)0x0) {
        r3 = r7;
        return;
    }
    r5 = (u32)lbl_80271500;
    r9 = *(u16*)((u8*)r4 + 0x0);
    while ((u32)r9 != (u32)0x0) {
        r8 = r5;
        for (r6 = 0x0; (s32)r6 < (s32)0x100; r6 = r6 + 0x1, r8 = r8 + 0x2) {
            r0 = *(u16*)((u8*)r8 + 0x0);
            if ((u32)r9 == (u32)r0) break;
        }
        if ((s32)r6 >= (s32)0x100) {
            r6 = 0xb7;
        }
        if ((u32)r3 != (u32)0x0) {
            r0 = r6 & 0xFF;
            *(u8*)((u8*)r3 + 0x0) = r0;
            r3 = r3 + 0x1;
        }
        r7 = r7 + 0x1;
        r4 = r4 + 0x2;
        r9 = *(u16*)((u8*)r4 + 0x0);
    }
    r3 = r7;
    return;
}

/* 0x800F9C04 | 0x100 */
void fn_800F9C04(void) {
    extern u8 lbl_80271300[];
    extern u8 lbl_80271500[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    if ((s32)r6 == (s32)0x1) {
        r7 = 0x0;
        if ((u32)r4 == (u32)0x0) {
            r3 = r7;
            return;
        }
        r6 = (u32)lbl_80271300;
        r6 = (u32)lbl_80271300;
        while ((s32)r5 != (s32)0x0) {
            r0 = *(u8*)((u8*)r4 + 0x0);
            if ((u32)r0 == (u32)0xff) break;
            if ((u32)r3 != (u32)0x0) {
                r0 = *(u8*)((u8*)r4 + 0x0);
                r4 = r4 + 0x1;
                r0 = r0 << 1;
                r0 = *(u16*)(r6 + r0);
                *(u16*)((u8*)r3 + 0x0) = r0;
                r3 = r3 + 0x2;
            }
            r7 = r7 + 0x1;
            /* subi r5, r5, 0x1 */;
        }
        if ((u32)r3 == (u32)0x0) { r3 = r7; return; }
        r0 = 0x0;
        *(u16*)((u8*)r3 + 0x0) = r0;
        r3 = r7;
        return;
    }
    /* default / case 7 and others */
    r7 = 0x0;
    if ((u32)r4 == (u32)0x0) {
        r3 = r7;
        return;
    }
    r6 = (u32)lbl_80271500;
    r6 = (u32)lbl_80271500;
    while ((s32)r5 != (s32)0x0) {
        r0 = *(u8*)((u8*)r4 + 0x0);
        if ((u32)r0 == (u32)0xff) break;
        if ((u32)r3 != (u32)0x0) {
            r0 = *(u8*)((u8*)r4 + 0x0);
            r4 = r4 + 0x1;
            r0 = r0 << 1;
            r0 = *(u16*)(r6 + r0);
            *(u16*)((u8*)r3 + 0x0) = r0;
            r3 = r3 + 0x2;
        }
        r7 = r7 + 0x1;
        /* subi r5, r5, 0x1 */;
    }
    if ((u32)r3 == (u32)0x0) { r3 = r7; return; }
    r0 = 0x0;
    *(u16*)((u8*)r3 + 0x0) = r0;

    r3 = r7;
    return;
}

/* 0x800F9D04 | 0x20 -- calls fn_80080ED8 */
extern void fn_80080ED8(void);
void fn_800F9D04(void) {
    fn_80080ED8();
}

/* 0x800F9D24 | 0x14C */
void fn_800F9D24(void) {
    extern void fn_800FE010();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    if ((s32)r30 <= (s32)0x0) {
        return;
    }
    r3 = r29;
    fn_800FE010();
    r0 = r3 + 0x1;
    r31 = (u32)r0 >> 1;
    if ((s32)r31 >= (s32)r30) {
        /* subi r31, r30, 0x1 */;
    }
    r3 = r28;
    r4 = r29;
    r5 = r31 << 1;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r6 = r31;
    r3 = r30 - r31;
    r5 = 0x0;
    if ((s32)r31 >= (s32)r30) { r3 = r28; return; }
    r0 = (u32)r3 >> 3;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 != (u32)0x0) {
        do {
            r4 = r6;
            r6 = r6 + 0x1;
            r0 = r4 << 1;
            r4 = r6;
            *(u16*)(r28 + r0) = r5;
            r0 = r4 << 1;
            r6 = r6 + 0x1;
            r4 = r6;
            *(u16*)(r28 + r0) = r5;
            r0 = r4 << 1;
            r6 = r6 + 0x1;
            r4 = r6;
            *(u16*)(r28 + r0) = r5;
            r0 = r4 << 1;
            r6 = r6 + 0x1;
            r4 = r6;
            *(u16*)(r28 + r0) = r5;
            r0 = r4 << 1;
            r6 = r6 + 0x1;
            r4 = r6;
            *(u16*)(r28 + r0) = r5;
            r0 = r4 << 1;
            r6 = r6 + 0x1;
            r4 = r6;
            *(u16*)(r28 + r0) = r5;
            r0 = r4 << 1;
            r6 = r6 + 0x1;
            r4 = r6;
            *(u16*)(r28 + r0) = r5;
            r0 = r4 << 1;
            r6 = r6 + 0x1;
            *(u16*)(r28 + r0) = r5;
        } while (--ctr != 0);
        r3 = r3 & 0x7;
        if ((u32)r0 == (u32)0x0) { r3 = r28; return; }
    }
    ctr_fn = (void(*)(void))r3;
    do {
        r4 = r6;
        r6 = r6 + 0x1;
        r0 = r4 << 1;
        *(u16*)(r28 + r0) = r5;
    } while (--ctr != 0);

    r3 = r28;

    return;
}

/* 0x74 | fn_800F9E70 | generic */
u32 fn_800F9E70(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_800FE010();
    memcpy();
    return 0;
}

/* 0x800F9EE4 | 0x180 */
void fn_800F9EE4(void) {
    extern void fn_800FDFE4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    r31 = r4;
    fn_800FDFE4();
    r29 = r3;
    r3 = r31;
    fn_800FDFE4();
    if ((u32)r29 == (u32)r3) {
        r4 = r31;
        r5 = r30;
        r6 = 0x0;
        ctr_fn = (void(*)(void))r3;
        if ((u32)r3 > (u32)0x0) {
            do {
                r3 = *(u16*)((u8*)r5 + 0x0);
                r0 = *(u16*)((u8*)r4 + 0x0);
                if ((u32)r3 != (u32)r0) {
                    r0 = r6 << 1;
                    r3 = *(u16*)(r30 + r0);
                    r0 = *(u16*)(r31 + r0);
                    if ((u32)r3 > (u32)r0) {
                        r3 = 0x1;
                        return;
                    }
                    r3 = -0x1;
                    return;
                }
                r4 = r4 + 0x2;
                r5 = r5 + 0x2;
                r6 = r6 + 0x1;
            } while (--ctr != 0);
        }
        r3 = 0x0;
        return;
    }
    if ((u32)r3 > (u32)r0) {
        r4 = r31;
        r5 = r30;
        r6 = 0x0;
        ctr_fn = (void(*)(void))r3;
        if ((u32)r3 > (u32)0x0) {
            do {
                r3 = *(u16*)((u8*)r5 + 0x0);
                r0 = *(u16*)((u8*)r4 + 0x0);
                if ((u32)r3 != (u32)r0) {
                    r0 = r6 << 1;
                    r3 = *(u16*)(r30 + r0);
                    r0 = *(u16*)(r31 + r0);
                    if ((u32)r3 > (u32)r0) {
                        r3 = 0x1;
                        return;
                    }
                    r3 = -0x1;
                    return;
                }
                r4 = r4 + 0x2;
                r5 = r5 + 0x2;
                r6 = r6 + 0x1;
            } while (--ctr != 0);
        }
        r3 = 0x1;
        return;
    }
    r4 = r31;
    r5 = r30;
    r6 = 0x0;
    ctr_fn = (void(*)(void))r29;
    if ((u32)r29 > (u32)0x0) {
        do {
            r3 = *(u16*)((u8*)r5 + 0x0);
            r0 = *(u16*)((u8*)r4 + 0x0);
            if ((u32)r3 != (u32)r0) {
                r0 = r6 << 1;
                r3 = *(u16*)(r30 + r0);
                r0 = *(u16*)(r31 + r0);
                if ((u32)r3 > (u32)r0) {
                    r3 = 0x1;
                    return;
                }
                r3 = -0x1;
                return;
            }
            r4 = r4 + 0x2;
            r5 = r5 + 0x2;
            r6 = r6 + 0x1;
        } while (--ctr != 0);
    }
    r3 = -0x1;

    return;
}

/* 0x800FA064 | 0xFC */
void fn_800FA064(void) {
    extern u8 lbl_8047CD10[];
    extern void fn_800FA444();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r31 = r3;
    r0 = *(s16*)((u8*)r31 + 0x18);
    if ((s32)r0 == (s32)0x0) return;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    fn_800FA444();
    r0 = *(u8*)((u8*)r31 + 0x4A);
    r3 = (u32)r3 >> 16;
    r5 = (s16)r3;
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 < (s32)0x1) {
            if ((s32)r0 < (s32)0x0) {
                return;
            }
            if ((s32)r0 >= (s32)0x3) return;
    r4 = *(s16*)((u8*)r31 + 0x18);
    r3 = (0x4330 << 16);
    /* xoris r0, r5, 0x8000 */;
    /* xoris r4, r4, 0x8000 */;
    f2 = *(f64*)lbl_8047CD10;
    f3 = *(f32*)((u8*)r31 + 0x4);
    f0 = *(f64*)(sp + 0x8);
    *(u32*)(sp + 0x14) = r0;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x10);
    f1 = f3 + f1;
    f0 = f0 - f2;
    f0 = f1 - f0;
    *(f32*)((u8*)r31 + 0xC) = f0;

    return;
}
}
}

/* 0x5C | fn_800FA160 | single_call_straight */
void fn_800FA160(void) {
    fn_800DBEB4();
}

/* 0x800FA1BC | 0xC4 */
void fn_800FA1BC(void) {
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern u8 lbl_80478B08;
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r6 = *(u32*)&lbl_80478B08;
    r5 = 0x0;
    r0 = *(u16*)((u8*)r6 + 0x4);
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) return;
    do {
        r4 = *(u32*)((u8*)r6 + 0x24);
        r0 = *(u16*)((u8*)r3 + 0x20);
        r7 = r4 + r5;
        r4 = *(u16*)((u8*)r7 + 0x0);
        if ((u32)r4 == (u32)r0) {
            r0 = *(u8*)((u8*)r7 + 0x2);
            *(u8*)((u8*)r3 + 0x22) = r0;
            r0 = *(u8*)((u8*)r7 + 0x3);
            *(u8*)((u8*)r3 + 0x23) = r0;
            r0 = *(u16*)((u8*)r3 + 0x20);
            if ((u32)r0 == (u32)0x0) {
                r0 = 0xb;
                *(u8*)((u8*)r3 + 0x42) = r0;
                return;
            }

            if ((u32)r0 == (u32)0x1 && (u32)r0 != (u32)0x1) {

                r0 = 0x6;
                *(u8*)((u8*)r3 + 0x42) = r0;
                return;
            }
            r4 = *(u8*)((u8*)r3 + 0x23);
            r0 = (0x4330 << 16);
            *(u32*)(sp + 0x8) = r0;
            f2 = *(f64*)lbl_8047CD28;
            f3 = *(f64*)lbl_8047CD20;
            f1 = *(f64*)(sp + 0x8);
            f0 = *(f64*)lbl_8047CD18;
            f1 = f1 - f2;
            f0 = f3 * f1 + f0;
            f0 = (f64)(s32)f0;
            *(f64*)(sp + 0x10) = f0;
            r0 = (s8)r0;
            *(u8*)((u8*)r3 + 0x42) = r0;
            return;
        }
        r5 = r5 + 0x8;
    } while (--ctr != 0);

    return;
}

/* 0x800FA280 | 0x94 */
void fn_800FA280(void) {
    extern u8 lbl_80478B08;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = *(u32*)&lbl_80478B08;
    r5 = (u32)r3 >> 20;
    r3 = r3 & 0xFFFFF;
    r9 = *(u32*)((u8*)r4 + 0x8);
    while ((u32)r9 != (u32)0x0) {
            r0 = *(u16*)((u8*)r9 + 0x0);
            if ((u32)r0 == (u32)r5) {
                r6 = *(u16*)((u8*)r9 + 0x4);
                r8 = r9 + 0x10;
                r7 = 0x0;
                goto L_800FA2F8;
                do {
                    r0 = r7 + r6;
                    r4 = (u32)r0 >> 1;
                    r10 = r4 << 3;
                    r0 = *(u32*)(r8 + r10);
                    if ((u32)r0 == (u32)r3) {
                        r3 = r8 + r10;
                        r0 = *(u32*)((u8*)r3 + 0x4);
                        r3 = r9 + r0;
                        return;
                    }
                    if ((u32)r0 < (u32)r3) {
                        r7 = r4 + 0x1;

                    } else {
                        r6 = r4;
                    }
                    L_800FA2F8: ;
                } while ((u32)r7 < (u32)r6);
            }
            r9 = *(u32*)((u8*)r9 + 0x8);
    }
    r3 = 0x0;
    return;
}

/* 0x800FA314 | 0xBC */
void fn_800FA314(void) {
    extern void fn_800FDFE4();
    extern u8 lbl_80478B08;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = *(u32*)&lbl_80478B08;
    r5 = (u32)r3 >> 20;
    r3 = r3 & 0xFFFFF;
    r9 = *(u32*)((u8*)r4 + 0x8);
    /* Walk linked list of sections */
    while ((u32)r9 != (u32)0x0) {
        r0 = *(u16*)((u8*)r9 + 0x0);
        if ((u32)r0 == (u32)r5) {
            r6 = *(u16*)((u8*)r9 + 0x4);
            r8 = r9 + 0x10;
            r7 = 0x0;
            /* Binary search within section */
            while ((u32)r7 < (u32)r6) {
                r0 = r7 + r6;
                r4 = (u32)r0 >> 1;
                r10 = r4 << 3;
                r0 = *(u32*)(r8 + r10);
                if ((u32)r0 == (u32)r3) {
                    r3 = r8 + r10;
                    r0 = *(u32*)((u8*)r3 + 0x4);
                    r3 = r9 + r0;
                    fn_800FDFE4();
                    return;
                }
                if ((u32)r0 < (u32)r3) {
                    r7 = r4 + 0x1;
                } else {
                    r6 = r4;
                }
            }
        }
        r9 = *(u32*)((u8*)r9 + 0x8);
    }
    r3 = 0x0;
    fn_800FDFE4();

    return;
}

/* 0x74 | fn_800FA3D0 | generic */
u32 fn_800FA3D0(u32 arg1, u32 arg2) {
    /* refs: lbl_80478B08 */
    return 0;
}

/* 0x800FA444 | 0x654 */
void fn_800FA444(void) {
    extern u8 lbl_80271700[];
    extern u8 lbl_80315678[];
    extern u8 lbl_80401E48[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern void fn_800FDF1C();
    extern u8 lbl_80478B08;
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    r28 = 0x0;
    r27 = 0x0;
    r26 = 0x0;
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((u32)r31 == (u32)0x0) {
        r25 = 0x0;

    } else {
        r3 = *(u32*)&lbl_80478B08;
        r4 = (u32)r31 >> 20;
        r5 = r31 & 0xFFFFF;
        r9 = *(u32*)((u8*)r3 + 0x8);
        while ((u32)r9 != (u32)0x0) {
                r0 = *(u16*)((u8*)r9 + 0x0);
                if ((u32)r0 == (u32)r4) {
                    r6 = *(u16*)((u8*)r9 + 0x4);
                    r8 = r9 + 0x10;
                    r7 = 0x0;
                    goto L_800FA4F8;
                    do {
                        r0 = r7 + r6;
                        r3 = (u32)r0 >> 1;
                        r10 = r3 << 3;
                        r0 = *(u32*)(r8 + r10);
                        if ((u32)r0 == (u32)r5) {
                            r0 = (u32)sp + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                            }
                            r3 = r8 + r10;
                            r0 = *(u32*)((u8*)r3 + 0x4);
                            r25 = r9 + r0;
                            goto L_800FA510;
                        }
                        if ((u32)r0 < (u32)0x0) {
                            r7 = r3 + 0x1;

                        } else {
                            r6 = r3;
                        }
                        L_800FA4F8: ;
                    } while ((u32)r7 < (u32)r6);
                }
                r9 = *(u32*)((u8*)r9 + 0x8);
        }
        r25 = 0x0;
    }
    L_800FA510: ;
    if ((u32)r25 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = (u32)lbl_80401E48;
    r4 = 0x0;
    r3 = (u32)lbl_80401E48;
    r5 = 0x68;
    r29 = r3;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = (u32)lbl_80401E48;
    f0 = *(f32*)lbl_8047CD08;
    r4 = (u32)lbl_80401E48;
    r5 = 0x1;
    r0 = -0x1;
    *(u8*)((u8*)r4 + 0x0) = r5;
    r6 = 0x0;
    *(f32*)((u8*)r4 + 0x60) = f0;
    r7 = *(u32*)&lbl_80478B08;
    *(f32*)((u8*)r4 + 0x64) = f0;
    *(u32*)((u8*)r4 + 0x24) = r0;
    *(u32*)((u8*)r4 + 0x28) = r25;
    *(u32*)((u8*)r4 + 0x2C) = r25;
    *(u32*)((u8*)r4 + 0x30) = r25;
    r0 = *(u8*)((u8*)r3 + 0x3);
    *(u16*)((u8*)r4 + 0x20) = r0;
    *(u32*)((u8*)r4 + 0x1C) = r31;
    *(u8*)((u8*)r4 + 0x1) = r5;
    r3 = *(u16*)((u8*)r7 + 0x4);
    r4 = r0 & 0xFFFF;
    ctr_fn = (void(*)(void))r3;
    if ((s32)r3 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r7 + 0x24);
            r3 = r0 + r6;
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u8*)((u8*)r3 + 0x2);
                *(u8*)((u8*)r29 + 0x22) = r0;
                r3 = *(u8*)((u8*)r3 + 0x3);
                *(u8*)((u8*)r29 + 0x23) = r3;
                if ((u32)r4 == (u32)0x0) {
                    r0 = 0xb;
                    *(u8*)((u8*)r29 + 0x42) = r0;
                    break;
                }

                if ((u32)r4 == (u32)0x1 && (u32)r4 != (u32)0x1) {

                    r0 = 0x6;
                    *(u8*)((u8*)r29 + 0x42) = r0;
                    break;
                }
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CD28;
                *(u32*)(sp + 0x10) = r0;
                f3 = *(f64*)lbl_8047CD20;
                f1 = *(f64*)(sp + 0x10);
                f0 = *(f64*)lbl_8047CD18;
                f1 = f1 - f2;
                f0 = f3 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x18) = f0;
                r0 = (s8)r0;
                *(u8*)((u8*)r29 + 0x42) = r0;
                break;
            }
            r6 = r6 + 0x8;
        } while (--ctr != 0);
    }
    L_800FA62C: ;
    r3 = *(u32*)((u8*)r29 + 0x30);
    r31 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r31 == (u32)0x0) {
        r3 = *(u8*)((u8*)r29 + 0x40);
        r0 = (s8)r3;
        if ((s32)r0 == (s32)0x0) {
    r3 = r31 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) {
        if ((u32)r3 == (u32)0xffff) {
            r3 = *(u32*)((u8*)r29 + 0x30);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r29 + 0x30) = r0;
            r5 = *(u8*)((u8*)r3 + 0x0);
            if ((u32)r5 == (u32)0x3) {
                r4 = *(u8*)((u8*)r29 + 0x22);
                r3 = (0x4330 << 16);
                r0 = (s16)r28;
                /* xoris r0, r0, 0x8000 */;
                f1 = *(f64*)lbl_8047CD28;
                f3 = *(f32*)((u8*)r29 + 0xC);
                f0 = *(f64*)(sp + 0x18);
                *(u32*)(sp + 0x14) = r0;
                f2 = f0 - f1;
                f1 = *(f64*)lbl_8047CD10;
                f0 = *(f64*)(sp + 0x10);
                f2 = f3 + f2;
                f0 = f0 - f1;
                *(f32*)((u8*)r29 + 0xC) = f2;
                if (f0 < f2) {
                    f0 = (f64)(s32)f2;
                    *(f64*)(sp + 0x18) = f0;
            }
            }
            r3 = *(u32*)&lbl_80478B08;
            r3 = *(u32*)((u8*)r3 + 0x28);
            if ((u32)r3 != (u32)0x0) {
                r0 = *(u8*)((u8*)r29 + 0x1);
                if ((u32)r0 == (u32)0x0) {
                    r0 = r5 << 3;
                    r0 = *(u8*)(r3 + r0);
                    /* extrwi r0, r0, 1, 27 */;
                } else {

                    r0 = r5 << 3;
                    r0 = *(u8*)(r3 + r0);
                    /* extrwi r0, r0, 1, 28 */;
                }
                if ((u32)r0 != (u32)0x0) {
                    r0 = r5 << 3;
                    r25 = r3 + r0;
                    r12 = *(u32*)((u8*)r25 + 0x4);
                    if ((u32)r12 != (u32)0x0) {
                        r3 = r29;
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        r0 = *(u8*)((u8*)r25 + 0x0);
                        /* extrwi r0, r0, 2, 24 */;
                        if ((u32)r0 != (u32)0x0 || (u32)r3 == (u32)0x0) {

                            if ((s32)r0 != (s32)0x2) {
                                if ((s32)r0 < (s32)0x2) {
                                    if ((s32)r0 < (s32)0x1) {

                                    } else {
                                        r30 = r3;

                                    }
                                } else {
                                        if ((u32)r3 == (u32)0x0) {
                                            r30 = 0x0;

                                        } else {
                                            r4 = *(u32*)&lbl_80478B08;
                                            r5 = (u32)r3 >> 20;
                                            r3 = r3 & 0xFFFFF;
                                            r9 = *(u32*)((u8*)r4 + 0x8);
                                            while ((u32)r9 != (u32)0x0) {
                                                    r0 = *(u16*)((u8*)r9 + 0x0);
                                                    if ((u32)r0 == (u32)r5) {
                                                        r6 = *(u16*)((u8*)r9 + 0x4);
                                                        r8 = r9 + 0x10;
                                                        r7 = 0x0;
                                                        goto L_800FA80C;
                                                        do {
                                                            r0 = r7 + r6;
                                                            r4 = (u32)r0 >> 1;
                                                            r0 = r4 << 3;
                                                            r10 = r8 + r0;
                                                            r0 = *(u32*)((u8*)r10 + 0x0);
                                                            if ((u32)r0 == (u32)r3) {
                                                                r0 = *(u32*)((u8*)r10 + 0x4);
                                                                r30 = r9 + r0;
                                                                goto L_800FA824;
                                                            }
                                                            if ((u32)r0 < (u32)r3) {
                                                                r7 = r4 + 0x1;

                                                            } else {
                                                                r6 = r4;
                                                            }
                                                            L_800FA80C: ;
                                                        } while ((u32)r7 < (u32)r6);
                                                    }
                                                    r9 = *(u32*)((u8*)r9 + 0x8);
                                            }
                                            r30 = 0x0;
                                            }
                                        }
                                        }
                            L_800FA824: ;
                            r4 = *(u8*)((u8*)r29 + 0x40);
                            r3 = (s8)r4;
                            if ((s32)r3 >= (s32)0x3) {
                                r3 = (u32)lbl_80271700;
                                r4 = (u32)lbl_80315678;
                                r3 = (u32)lbl_80271700;
                                r4 = (u32)lbl_80315678;
                                /* crclr cr1eq */;
                                ((void(*)(void))fn_800DD970)();

                            } else {
                                r0 = r4 + 0x1;
                                r3 = r3 << 2;
                                *(u8*)((u8*)r29 + 0x40) = r0;
                                r0 = r3 + 0x34;
                                r3 = *(u32*)((u8*)r29 + 0x30);
                                *(u32*)(r29 + r0) = r3;
                                *(u32*)((u8*)r29 + 0x30) = r30;
                            }
                            }
                            }
                            }
                            }

            r0 = r26 & 0xFF;
            if ((u32)r0 == (u32)0x0) goto L_800FA9A8;
            f2 = *(f32*)((u8*)r29 + 0xC);
            f0 = *(f32*)((u8*)r29 + 0x4);
            if (f2 != f0) goto L_800FA9A8;
            r3 = *(u8*)((u8*)r29 + 0x22);
            r0 = (0x4330 << 16);
            *(u32*)(sp + 0x18) = r0;
            /* xoris r0, r3, 0x8000 */;
            f1 = *(f64*)lbl_8047CD10;
            *(u32*)(sp + 0x1C) = r0;
            f0 = *(f64*)(sp + 0x18);
            f0 = f0 - f1;
            f0 = f2 + f0;
            *(f32*)((u8*)r29 + 0xC) = f0;

        } else {
            r0 = *(u8*)((u8*)r29 + 0x4B);
            if ((u32)r0 == (u32)0x2) goto L_800FA62C;
            if ((u32)r3 == (u32)0x20) {
                r3 = *(u8*)((u8*)r29 + 0x22);
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x18) = r0;
                r0 = (u32)r3 >> 1;
                f2 = *(f64*)lbl_8047CD10;
                /* xoris r0, r0, 0x8000 */;
                f0 = *(f32*)((u8*)r29 + 0x60);
                *(u32*)(sp + 0x1C) = r0;
                f1 = *(f64*)(sp + 0x18);
                f1 = f1 - f2;
                f0 = f1 * f0;
                *(f32*)((u8*)r29 + 0x14) = f0;

            } else {
                r3 = r29;
                r4 = r31;
                r5 = 0x0;
                fn_800FDF1C();
                if ((u32)r3 == (u32)0x0) {
                    r3 = *(u8*)((u8*)r29 + 0x22);
                    r0 = (0x4330 << 16);
                    *(u32*)(sp + 0x18) = r0;
                    f2 = *(f64*)lbl_8047CD28;
                    f0 = *(f32*)((u8*)r29 + 0x60);
                    f1 = *(f64*)(sp + 0x18);
                    f1 = f1 - f2;
                    f0 = f1 * f0;
                    *(f32*)((u8*)r29 + 0x14) = f0;

                } else {
                    r3 = *(u8*)((u8*)r3 + 0x2);
                    r0 = (0x4330 << 16);
                    *(u32*)(sp + 0x18) = r0;
                    f2 = *(f64*)lbl_8047CD28;
                    f0 = *(f32*)((u8*)r29 + 0x60);
                    f1 = *(f64*)(sp + 0x18);
                    f1 = f1 - f2;
                    f0 = f1 * f0;
                    *(f32*)((u8*)r29 + 0x14) = f0;
                }
                }

            r0 = *(u8*)((u8*)r29 + 0x41);
            r0 = (s8)r0;
            if ((s32)r0 == (s32)0x0) {
                r0 = r31 & 0xFFFF;
                if ((u32)r0 == (u32)0x300c) {
                    r26 = 0x1;
                }
                r0 = r31 & 0xFFFF;
                if ((u32)r0 == (u32)0x300d) {
                    r26 = 0x0;
            }
            }
            f1 = *(f32*)((u8*)r29 + 0xC);
            f0 = *(f32*)((u8*)r29 + 0x14);
            f0 = f1 + f0;
            *(f32*)((u8*)r29 + 0xC) = f0;
        }
        L_800FA9A8: ;
        r3 = (s16)r28;
        r0 = (0x4330 << 16);
        /* xoris r3, r3, 0x8000 */;
        *(u32*)(sp + 0x18) = r0;
        f1 = *(f64*)lbl_8047CD10;
        f2 = *(f32*)((u8*)r29 + 0xC);
        f0 = *(f64*)(sp + 0x18);
        f0 = f0 - f1;
        if (f0 < f2) {
            f0 = (f64)(s32)f2;
            *(f64*)(sp + 0x10) = f0;
        }
        r3 = (s16)r27;
        r0 = (0x4330 << 16);
        /* xoris r3, r3, 0x8000 */;
        *(u32*)(sp + 0x20) = r0;
        f1 = *(f64*)lbl_8047CD10;
        f2 = *(f32*)((u8*)r29 + 0x10);
        f0 = *(f64*)(sp + 0x20);
        f0 = f0 - f1;
        if (f0 >= f2) goto L_800FA62C;
        f0 = (f64)(s32)f2;
        *(f64*)(sp + 0x20) = f0;
        goto L_800FA62C;
    }
    r0 = *(u8*)((u8*)r29 + 0x23);
    r4 = (0x4330 << 16);
    r5 = (s16)r27;
    r3 = (s16)r28;
    *(u32*)(sp + 0x1C) = r0;
    /* xoris r5, r5, 0x8000 */;
    /* subi r0, r3, 0x1 */;
    f1 = *(f64*)lbl_8047CD28;
    r3 = r0 << 16;
    f4 = *(f64*)lbl_8047CD10;
    f0 = *(f64*)(sp + 0x18);
    f2 = f0 - f1;
    f1 = *(f32*)((u8*)r29 + 0x64);
    f0 = *(f32*)lbl_8047CD08;
    f3 = *(f64*)(sp + 0x20);
    f0 = f2 * f1 + f0;
    f1 = f3 - f4;
    f0 = f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x10) = f0;
    r0 = (s16)r27;
    r3 = r3 | r0;

    return;
}
}
}

/* 0x800FAA98 | 0x460 */
void fn_800FAA98(void) {
    extern u8 lbl_80271700[];
    extern u8 lbl_80315678[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD28[];
    extern u8 lbl_8047CD30[];
    extern u8 lbl_8047CD34[];
    extern u8 lbl_8047CD38[];
    extern void fn_800FDF1C();
    extern u8 lbl_80478B08;
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r23 = r3;
    r7 = *(u32*)((u8*)r23 + 0x34);
    r3 = 0x0;
    r6 = *(u32*)((u8*)r23 + 0x38);
    r0 = 0x1;
    r5 = *(u32*)((u8*)r23 + 0x3C);
    r31 = 0x0;
    r4 = *(u8*)((u8*)r23 + 0x45);
    r30 = 0x0;
    r25 = *(u32*)((u8*)r23 + 0x30);
    r28 = 0x0;
    r24 = *(u8*)((u8*)r23 + 0x40);
    r21 = (s8)r4;
    r26 = *(u8*)((u8*)r23 + 0x1);
    r27 = 0x0;
    r29 = 0x0;
    *(u32*)((u8*)r23 + 0x54) = r3;
    *(u8*)((u8*)r23 + 0x58) = r3;
    *(u8*)((u8*)r23 + 0x59) = r3;
    *(u8*)((u8*)r23 + 0x1) = r3;
    *(u8*)((u8*)r23 + 0x45) = r0;
    *(u8*)((u8*)r23 + 0x4B) = r0;
    goto L_800FAE14;
    L_800FAB10: ;
    r3 = *(u32*)((u8*)r23 + 0x30);
    r4 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r4 == (u32)0x0) {
        r3 = *(u8*)((u8*)r23 + 0x40);
        r0 = (s8)r3;
        if ((s32)r0 == (s32)0x0) {
    r0 = r4 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) {
        if ((u32)r0 == (u32)0xffff) {
            r4 = *(u32*)((u8*)r23 + 0x30);
            r0 = r4 + 0x1;
            *(u32*)((u8*)r23 + 0x30) = r0;
            r3 = *(u32*)&lbl_80478B08;
            r4 = *(u8*)((u8*)r4 + 0x0);
            r3 = *(u32*)((u8*)r3 + 0x28);
            if ((u32)r3 != (u32)0x0) {
                r0 = *(u8*)((u8*)r23 + 0x1);
                if ((u32)r0 == (u32)0x0) {
                    r0 = r4 << 3;
                    r0 = *(u8*)(r3 + r0);
                    /* extrwi r0, r0, 1, 27 */;
                } else {

                    r0 = r4 << 3;
                    r0 = *(u8*)(r3 + r0);
                    /* extrwi r0, r0, 1, 28 */;
                }
                if ((u32)r0 != (u32)0x0) {
                    r0 = r4 << 3;
                    r20 = r3 + r0;
                    r12 = *(u32*)((u8*)r20 + 0x4);
                    if ((u32)r12 != (u32)0x0) {
                        r3 = r23;
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        r0 = *(u8*)((u8*)r20 + 0x0);
                        /* extrwi r0, r0, 2, 24 */;
                        if ((u32)r0 != (u32)0x0 || (u32)r3 == (u32)0x0) {

                            if ((s32)r0 != (s32)0x2) {
                                if ((s32)r0 < (s32)0x2) {
                                    if ((s32)r0 < (s32)0x1) {

                                    } else {
                                        r22 = r3;

                                    }
                                } else {
                                        if ((u32)r3 == (u32)0x0) {
                                            r22 = 0x0;

                                        } else {
                                            r4 = *(u32*)&lbl_80478B08;
                                            r5 = (u32)r3 >> 20;
                                            r3 = r3 & 0xFFFFF;
                                            r9 = *(u32*)((u8*)r4 + 0x8);
                                            while ((u32)r9 != (u32)0x0) {
                                                    r0 = *(u16*)((u8*)r9 + 0x0);
                                                    if ((u32)r0 == (u32)r5) {
                                                        r6 = *(u16*)((u8*)r9 + 0x4);
                                                        r8 = r9 + 0x10;
                                                        r7 = 0x0;
                                                        goto L_800FAC90;
                                                        do {
                                                            r0 = r7 + r6;
                                                            r4 = (u32)r0 >> 1;
                                                            r0 = r4 << 3;
                                                            r10 = r8 + r0;
                                                            r0 = *(u32*)((u8*)r10 + 0x0);
                                                            if ((u32)r0 == (u32)r3) {
                                                                r0 = *(u32*)((u8*)r10 + 0x4);
                                                                r22 = r9 + r0;
                                                                goto L_800FACA8;
                                                            }
                                                            if ((u32)r0 < (u32)r3) {
                                                                r7 = r4 + 0x1;

                                                            } else {
                                                                r6 = r4;
                                                            }
                                                            L_800FAC90: ;
                                                        } while ((u32)r7 < (u32)r6);
                                                    }
                                                    r9 = *(u32*)((u8*)r9 + 0x8);
                                            }
                                            r22 = 0x0;
                                            }
                                        }
                                        }
                            L_800FACA8: ;
                            r5 = *(u8*)((u8*)r23 + 0x40);
                            r3 = (s8)r5;
                            if ((s32)r3 >= (s32)0x3) {
                                r3 = (u32)lbl_80271700;
                                r4 = (u32)lbl_80315678;
                                r3 = (u32)lbl_80271700;
                                r4 = (u32)lbl_80315678;
                                /* crclr cr1eq */;
                                ((void(*)(void))fn_800DD970)();

                            } else {
                                r4 = *(u32*)((u8*)r23 + 0x30);
                                r0 = r5 + 0x1;
                                r3 = r3 << 2;
                                *(u8*)((u8*)r23 + 0x40) = r0;
                                r0 = r3 + 0x34;
                                *(u32*)(r23 + r0) = r4;
                                *(u32*)((u8*)r23 + 0x30) = r22;
                            }
                            }
                            }
                            }
                            }

            r0 = r29 & 0xFF;
            if ((u32)r0 == (u32)0x0) {
                r0 = *(u8*)((u8*)r23 + 0x4B);
                if ((u32)r0 != (u32)0x2) goto L_800FAB10;
                r0 = *(u32*)((u8*)r23 + 0x30);
                *(u32*)((u8*)r23 + 0x54) = r0;

            } else {
                r0 = *(u8*)((u8*)r23 + 0x4B);
                if ((u32)r0 == (u32)0x0) goto L_800FAE10;
                goto L_800FAB10;
            }
            r3 = r23;
            r5 = 0x0;
            fn_800FDF1C();
            if ((u32)r3 != (u32)0x0) {
                r3 = *(u8*)((u8*)r3 + 0x2);
            } else {

                r3 = *(u8*)((u8*)r23 + 0x22);
            }
            r0 = r29 & 0xFF;
            if ((u32)r0 == (u32)0x0) {
                r0 = (s16)r3;
                r3 = (0x4330 << 16);
                /* xoris r0, r0, 0x8000 */;
                r4 = (s16)r28;
                *(u32*)(sp + 0x24) = r0;
                /* xoris r0, r4, 0x8000 */;
                f4 = *(f64*)lbl_8047CD10;
                r31 = r31 + 0x1;
                f1 = *(f32*)((u8*)r23 + 0x60);
                f0 = *(f64*)(sp + 0x20);
                *(u32*)(sp + 0x1C) = r0;
                f2 = f0 - f4;
                f0 = *(f32*)lbl_8047CD30;
                f3 = *(f64*)(sp + 0x18);
                f0 = f2 * f1 + f0;
                f1 = f3 - f4;
                f0 = f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x28) = f0;
                goto L_800FAB10;
            }
            r0 = (s16)r3;
            r3 = (0x4330 << 16);
            /* xoris r0, r0, 0x8000 */;
            r4 = (s16)r27;
            *(u32*)(sp + 0x24) = r0;
            /* xoris r0, r4, 0x8000 */;
            f4 = *(f64*)lbl_8047CD10;
            r30 = r30 + 0x1;
            f1 = *(f32*)((u8*)r23 + 0x60);
            f0 = *(f64*)(sp + 0x20);
            *(u32*)(sp + 0x2C) = r0;
            f2 = f0 - f4;
            f3 = *(f32*)lbl_8047CD34;
            f0 = *(f32*)lbl_8047CD30;
            f1 = f2 * f1;
            f2 = *(f64*)(sp + 0x28);
            f2 = f2 - f4;
            f0 = f3 * f1 + f0;
            f0 = f2 + f0;
            f0 = (f64)(s32)f0;
            *(f64*)(sp + 0x18) = f0;
            goto L_800FAB10;
        }
            }
    L_800FAE10: ;
    r29 = r29 + 0x1;
    L_800FAE14: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_800FAB10;
    r3 = (s16)r27;
    r0 = (s16)r28;
    r3 = r0 - r3;
    r4 = (0x4330 << 16);
    r0 = (u32)r3 >> 31;
    r5 = *(u8*)((u8*)r23 + 0x22);
    r0 = r0 + r3;
    f0 = *(f32*)((u8*)r23 + 0x60);
    r0 = (s32)r0 >> 1;
    /* xoris r3, r0, 0x8000 */;
    f5 = *(f64*)lbl_8047CD28;
    r0 = r21 & 0xFF;
    f1 = *(f64*)lbl_8047CD10;
    *(u8*)((u8*)r23 + 0x5A) = r31;
    f2 = *(f64*)(sp + 0x28);
    *(u8*)((u8*)r23 + 0x5B) = r30;
    f2 = f2 - f5;
    f6 = *(f32*)lbl_8047CD34;
    f3 = *(f32*)((u8*)r23 + 0xC);
    f4 = f2 * f0;
    f2 = *(f32*)lbl_8047CD38;
    f0 = *(f64*)(sp + 0x20);
    f4 = f6 * f4;
    f0 = f0 - f1;
    f0 = f3 + f0;
    *(f32*)((u8*)r23 + 0x4C) = f0;
    *(f32*)((u8*)r23 + 0x5C) = f4;
    r6 = *(u8*)((u8*)r23 + 0x23);
    f0 = *(f32*)((u8*)r23 + 0x10);
    f1 = *(f64*)(sp + 0x18);
    f1 = f1 - f5;
    f0 = -(f2 * f1 - f0);
    *(f32*)((u8*)r23 + 0x50) = f0;
    *(u32*)((u8*)r23 + 0x30) = r25;
    *(u8*)((u8*)r23 + 0x40) = r24;
    *(u32*)((u8*)r23 + 0x34) = r5;
    *(u32*)((u8*)r23 + 0x38) = r4;
    *(u32*)((u8*)r23 + 0x3C) = r3;
    *(u8*)((u8*)r23 + 0x45) = r0;
    *(u8*)((u8*)r23 + 0x1) = r26;
    return;
}
}
}

/* 0x800FAEF8 | 0x544 */
void fn_800FAEF8(void) {
    extern u8 lbl_80314E08[];
    extern u8 lbl_80314F98[];
    extern u8 lbl_80401DE0[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern u8 lbl_8047CD30[];
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800D9ED8();
    extern void fn_800DC1D4();
    extern void fn_800DE680();
    extern void fn_800EF504();
    extern void fn_800EF548();
    extern void fn_800FD69C();
    extern void fn_800FDF1C();
    extern void fn_800FE4D4();
    extern u8 lbl_80478B08;
    u8 sp[0xE0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r27 = r4;
    r29 = r5;
    if ((s32)r0 == (s32)0) {
        *(f64*)(sp + 0x28) = f1;
        *(f64*)(sp + 0x30) = f2;
        *(f64*)(sp + 0x38) = f3;
        *(f64*)(sp + 0x40) = f4;
        *(f64*)(sp + 0x48) = f5;
        *(f64*)(sp + 0x50) = f6;
        *(f64*)(sp + 0x58) = f7;
        *(f64*)(sp + 0x60) = f8;
    }
    r4 = (u32)sp + 0xe8;
    r0 = (u32)sp + 0x8;
    r5 = (u32)lbl_80401DE0;
    r3 = (0x400 << 16);
    r31 = (u32)lbl_80401DE0;
    r7 = (u32)sp + 0x6c;
    r5 = r6;
    r6 = r7;
    r3 = r31 + 0x4d0;
    r4 = 0xff;
    *(u32*)(sp + 0x74) = r0;
    fn_800DE680();
    r4 = r31 + 0x4d0;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0xFF) = r0;
    r3 = r31 + 0xd0;
    ((void(*)(void))fn_80080ED8)();
    r30 = r31 + 0x5d0;
    r4 = 0x0;
    r3 = r30;
    r5 = 0x68;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (0x4330 << 16);
    /* xoris r0, r28, 0x8000 */;
    *(u32*)(sp + 0x7C) = r0;
    /* xoris r3, r27, 0x8000 */;
    r8 = 0x1;
    r7 = r30;
    r5 = r31 + 0xd0;
    f2 = *(f64*)lbl_8047CD10;
    r6 = -0x1;
    f0 = *(f64*)(sp + 0x78);
    r0 = 0x2;
    r9 = 0x0;
    f1 = f0 - f2;
    f3 = *(f32*)lbl_8047CD08;
    *(u32*)((u8*)r7 + 0x24) = r6;
    r6 = *(u32*)&lbl_80478B08;
    f0 = *(f64*)(sp + 0x80);
    *(u8*)((u8*)r31 + 0x5D0) = r8;
    f0 = f0 - f2;
    *(f32*)((u8*)r7 + 0x60) = f3;
    *(f32*)((u8*)r7 + 0x64) = f3;
    *(u32*)((u8*)r7 + 0x28) = r5;
    *(u32*)((u8*)r7 + 0x2C) = r5;
    *(u32*)((u8*)r7 + 0x30) = r5;
    *(f32*)((u8*)r7 + 0x4) = f1;
    *(f32*)((u8*)r7 + 0x8) = f0;
    *(u32*)((u8*)r7 + 0x24) = r29;
    *(u8*)((u8*)r7 + 0x2) = r8;
    *(u16*)((u8*)r7 + 0x20) = r0;
    r3 = *(u16*)((u8*)r6 + 0x4);
    r4 = r0 & 0xFFFF;
    ctr_fn = (void(*)(void))r3;
    if ((s32)r3 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r6 + 0x24);
            r3 = r0 + r9;
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u8*)((u8*)r3 + 0x2);
                *(u8*)((u8*)r30 + 0x22) = r0;
                r3 = *(u8*)((u8*)r3 + 0x3);
                *(u8*)((u8*)r30 + 0x23) = r3;
                if ((u32)r4 == (u32)0x0) {
                    r0 = 0xb;
                    *(u8*)((u8*)r30 + 0x42) = r0;
                    break;
                }

                if ((u32)r4 == (u32)0x1 && (u32)r4 != (u32)0x1) {

                    r0 = 0x6;
                    *(u8*)((u8*)r30 + 0x42) = r0;
                    break;
                }
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CD28;
                *(u32*)(sp + 0x80) = r0;
                f3 = *(f64*)lbl_8047CD20;
                f1 = *(f64*)(sp + 0x80);
                f0 = *(f64*)lbl_8047CD18;
                f1 = f1 - f2;
                f0 = f3 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x78) = f0;
                r0 = (s8)r0;
                *(u8*)((u8*)r30 + 0x42) = r0;
                break;
            }
            r9 = r9 + 0x8;
        } while (--ctr != 0);
    }
    fn_800FE4D4();
    r3 = 0x1;
    fn_800D9ED8();
    r3 = 0x3;
    fn_800D88DC();
    r3 = 0x4;
    fn_800D888C();
    r3 = (u32)lbl_80314F98;
    r3 = (u32)lbl_80314F98;
    fn_800D7820();
    r4 = *(u32*)&lbl_80478B08;
    r3 = 0x0;
    r0 = *(u8*)((u8*)r4 + 0x1D);
    r0 = (s8)r0;
    r0 = r0 << 2;
    r4 = r4 + r0;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_800D85D4();
    r3 = *(u32*)&lbl_80478B08;
    r4 = 0x0;
    r0 = *(u8*)((u8*)r3 + 0x1D);
    r0 = (s8)r0;
    r0 = r0 << 2;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_800EF548();
    r4 = *(u32*)&lbl_80478B08;
    *(u32*)((u8*)r4 + 0x14) = r3;
    f1 = *(f32*)((u8*)r30 + 0x4);
    f0 = *(f32*)((u8*)r30 + 0x8);
    *(f32*)((u8*)r30 + 0xC) = f1;
    *(f32*)((u8*)r30 + 0x10) = f0;
    L_800FB160: ;
    r3 = *(u32*)((u8*)r30 + 0x30);
    r4 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r4 == (u32)0x0) {
        r3 = *(u8*)((u8*)r30 + 0x40);
        r0 = (s8)r3;
        if ((s32)r0 == (s32)0x0) {
    r0 = r4 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) {
        if ((u32)r0 == (u32)0xa || (u32)r0 == (u32)0xd) goto L_800FB160;

        if ((u32)r0 == (u32)0x20) {
            r3 = *(u8*)((u8*)r30 + 0x22);
            r0 = (0x4330 << 16);
            *(u32*)(sp + 0x80) = r0;
            r0 = (u32)r3 >> 1;
            f2 = *(f64*)lbl_8047CD10;
            /* xoris r0, r0, 0x8000 */;
            f0 = *(f32*)((u8*)r30 + 0x60);
            *(u32*)(sp + 0x84) = r0;
            f1 = *(f64*)(sp + 0x80);
            f1 = f1 - f2;
            f0 = f1 * f0;
            *(f32*)((u8*)r30 + 0x14) = f0;

        } else {
            r3 = r30;
            r5 = (u32)sp + 0x68;
            fn_800FDF1C();
            r6 = r3;
            if ((u32)r6 == (u32)0x0) {
                f0 = *(f32*)((u8*)r30 + 0x10);
                r6 = (0x4330 << 16);
                f1 = *(f32*)((u8*)r30 + 0xC);
                r3 = (0x8000 << 16);
                f0 = (f64)(s32)f0;
                r0 = *(u8*)((u8*)r30 + 0x22);
                f1 = (f64)(s32)f1;
                r4 = *(u8*)((u8*)r30 + 0x23);
                *(u32*)(sp + 0x8C) = r0;
                r3 = r3 + 0x2;
                *(f64*)(sp + 0x78) = f0;
                f6 = *(f64*)lbl_8047CD28;
                *(f64*)(sp + 0x80) = f1;
                r0 = r5 + 0x2;
                r31 = (s16)r0;
                r0 = (s16)r27;
                /* xoris r5, r0, 0x8000 */;
                /* xoris r0, r31, 0x8000 */;
                f0 = *(f64*)(sp + 0x88);
                f4 = *(f64*)lbl_8047CD10;
                f5 = f0 - f6;
                f0 = *(f64*)(sp + 0x90);
                f2 = f0 - f4;
                f3 = *(f32*)((u8*)r30 + 0x60);
                f1 = *(f32*)((u8*)r30 + 0x64);
                f0 = *(f64*)(sp + 0xA0);
                f3 = f5 * f3 + f2;
                *(u32*)(sp + 0xAC) = r0;
                f2 = f0 - f6;
                f3 = (f64)(s32)f3;
                f0 = *(f64*)(sp + 0xA8);
                *(f64*)(sp + 0x98) = f3;
                f0 = f0 - f4;
                f0 = f2 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0xB0) = f0;
                fn_800D888C();
                r3 = 0x7;
                fn_800D6A00();
                r3 = (u32)lbl_80314E08;
                r3 = (u32)lbl_80314E08;
                fn_800D7820();
                r3 = 0x2;
                fn_800D67BC();
                r3 = r27;
                r4 = r31;
                fn_800D61E4();
                r3 = 0x0;
                r4 = 0xff;
                r5 = 0xff;
                r6 = 0xff;
                r7 = 0xff;
                fn_800D5CB8();
                r3 = r28;
                r4 = r29;
                fn_800D61E4();
                r3 = 0x0;
                r4 = 0xff;
                r5 = 0xff;
                r6 = 0xff;
                r7 = 0xff;
                fn_800D5CB8();
                fn_800D6728();
                r3 = (0x8000 << 16);
                r3 = r3 + 0x2;
                fn_800D88DC();
                r3 = (u32)lbl_80314F98;
                r3 = (u32)lbl_80314F98;
                fn_800D7820();
                r3 = 0x1;
                fn_800DC1D4();
                r3 = *(u8*)((u8*)r30 + 0x22);
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0xB8) = r0;
                f2 = *(f64*)lbl_8047CD28;
                f0 = *(f32*)((u8*)r30 + 0x60);
                f1 = *(f64*)(sp + 0xB8);
                f3 = *(f32*)lbl_8047CD30;
                f1 = f1 - f2;
                f0 = f1 * f0;
                f0 = f3 + f0;
                *(f32*)((u8*)r30 + 0x14) = f0;

            } else {
                r3 = r30;
                r0 = *(u32*)((u8*)r6 + 0x4);
                r5 = *(u32*)((u8*)r8 + 0x4);
                r4 = r0 & 0xFFFFFF;
                r29 = *(u8*)((u8*)r6 + 0x2);
                r4 = r5 + r4;
                r0 = (u32)r0 >> 24;
                r6 = *(u8*)((u8*)r6 + 0x3);
                r7 = (s8)r0;
                r5 = r29;
                r4 = r8 + r4;
                fn_800FD69C();
                r3 = (s16)r29;
                r0 = (0x4330 << 16);
                /* xoris r3, r3, 0x8000 */;
                *(u32*)(sp + 0xB8) = r0;
                f2 = *(f64*)lbl_8047CD10;
                f0 = *(f32*)((u8*)r30 + 0x60);
                f1 = *(f64*)(sp + 0xB8);
                f1 = f1 - f2;
                f0 = f1 * f0;
                *(f32*)((u8*)r30 + 0x14) = f0;
            }
            }

        f1 = *(f32*)((u8*)r30 + 0xC);
        f0 = *(f32*)((u8*)r30 + 0x14);
        f0 = f1 + f0;
        *(f32*)((u8*)r30 + 0xC) = f0;
        goto L_800FB160;
    }
    r3 = *(u32*)&lbl_80478B08;
    r0 = *(u8*)((u8*)r3 + 0x1D);
    r0 = (s8)r0;
    r0 = r0 << 2;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_800EF504();
    r3 = 0x0;
    return;
}
}
}

/* 0x800FB43C | 0x244 */
void fn_800FB43C(void) {
    extern u8 lbl_80402418[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern void fn_800FC7E0();
    extern u8 lbl_80478B08;
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = (u32)lbl_80402418;
    r31 = (u32)lbl_80402418;
    if ((u32)r30 == (u32)0x0) {
        r27 = 0x0;

    } else {
        r3 = *(u32*)&lbl_80478B08;
        r4 = (u32)r30 >> 20;
        r5 = r30 & 0xFFFFF;
        r9 = *(u32*)((u8*)r3 + 0x8);
        while ((u32)r9 != (u32)0x0) {
                r0 = *(u16*)((u8*)r9 + 0x0);
                if ((u32)r0 == (u32)r4) {
                    r6 = *(u16*)((u8*)r9 + 0x4);
                    r8 = r9 + 0x10;
                    r7 = 0x0;
                    goto L_800FB4E8;
                    do {
                        r0 = r7 + r6;
                        r3 = (u32)r0 >> 1;
                        r10 = r3 << 3;
                        r0 = *(u32*)(r8 + r10);
                        if ((u32)r0 == (u32)r5) {
                            r0 = (u32)sp + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                            }
                            r3 = r8 + r10;
                            r0 = *(u32*)((u8*)r3 + 0x4);
                            r27 = r9 + r0;
                            goto L_800FB500;
                        }
                        if ((u32)r0 < (u32)0x0) {
                            r7 = r3 + 0x1;

                        } else {
                            r6 = r3;
                        }
                        L_800FB4E8: ;
                    } while ((u32)r7 < (u32)r6);
                }
                r9 = *(u32*)((u8*)r9 + 0x8);
        }
        r27 = 0x0;
    }
    L_800FB500: ;
    if ((u32)r27 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x68;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (0x4330 << 16);
    /* xoris r0, r28, 0x8000 */;
    *(u32*)(sp + 0x14) = r0;
    /* xoris r3, r29, 0x8000 */;
    f3 = *(f32*)lbl_8047CD08;
    r6 = 0x1;
    r5 = -0x1;
    f2 = *(f64*)lbl_8047CD10;
    r7 = 0x0;
    f0 = *(f64*)(sp + 0x10);
    r0 = 0x3;
    f1 = f0 - f2;
    r8 = *(u32*)&lbl_80478B08;
    f0 = *(f64*)(sp + 0x18);
    *(u8*)((u8*)r31 + 0x0) = r6;
    f0 = f0 - f2;
    *(f32*)((u8*)r31 + 0x60) = f3;
    *(f32*)((u8*)r31 + 0x64) = f3;
    *(u32*)((u8*)r31 + 0x24) = r5;
    *(u32*)((u8*)r31 + 0x28) = r27;
    *(u32*)((u8*)r31 + 0x2C) = r27;
    *(u32*)((u8*)r31 + 0x30) = r27;
    r3 = *(u8*)((u8*)r3 + 0x3);
    *(u16*)((u8*)r31 + 0x20) = r3;
    *(u32*)((u8*)r31 + 0x1C) = r30;
    *(f32*)((u8*)r31 + 0x4) = f1;
    *(f32*)((u8*)r31 + 0x8) = f0;
    *(u16*)((u8*)r31 + 0x18) = r7;
    *(u16*)((u8*)r31 + 0x1A) = r7;
    *(u8*)((u8*)r31 + 0x44) = r0;
    *(u32*)((u8*)r31 + 0x24) = r5;
    *(u8*)((u8*)r31 + 0x2) = r6;
    r0 = *(u16*)((u8*)r8 + 0x4);
    r4 = r3 & 0xFFFF;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r8 + 0x24);
            r3 = r0 + r7;
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u8*)((u8*)r3 + 0x2);
                *(u8*)((u8*)r31 + 0x22) = r0;
                r3 = *(u8*)((u8*)r3 + 0x3);
                *(u8*)((u8*)r31 + 0x23) = r3;
                if ((u32)r4 == (u32)0x0) {
                    r0 = 0xb;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }

                if ((u32)r4 == (u32)0x1 && (u32)r4 != (u32)0x1) {

                    r0 = 0x6;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CD28;
                *(u32*)(sp + 0x18) = r0;
                f3 = *(f64*)lbl_8047CD20;
                f1 = *(f64*)(sp + 0x18);
                f0 = *(f64*)lbl_8047CD18;
                f1 = f1 - f2;
                f0 = f3 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x10) = f0;
                r0 = (s8)r0;
                *(u8*)((u8*)r31 + 0x42) = r0;
                break;
            }
            r7 = r7 + 0x8;
        } while (--ctr != 0);
    }
    r4 = *(u8*)((u8*)r31 + 0x44);
    r3 = r31;
    r5 = 0x0;
    r6 = 0x0;
    fn_800FC7E0();

    return;
}

/* 0x800FB680 | 0x248 */
void fn_800FB680(void) {
    extern u8 lbl_80402418[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern void fn_800FC7E0();
    extern u8 lbl_80478B08;
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = (u32)lbl_80402418;
    r31 = (u32)lbl_80402418;
    if ((u32)r30 == (u32)0x0) {
        r26 = 0x0;

    } else {
        r3 = *(u32*)&lbl_80478B08;
        r4 = (u32)r30 >> 20;
        r5 = r30 & 0xFFFFF;
        r9 = *(u32*)((u8*)r3 + 0x8);
        while ((u32)r9 != (u32)0x0) {
                r0 = *(u16*)((u8*)r9 + 0x0);
                if ((u32)r0 == (u32)r4) {
                    r6 = *(u16*)((u8*)r9 + 0x4);
                    r8 = r9 + 0x10;
                    r7 = 0x0;
                    goto L_800FB730;
                    do {
                        r0 = r7 + r6;
                        r3 = (u32)r0 >> 1;
                        r10 = r3 << 3;
                        r0 = *(u32*)(r8 + r10);
                        if ((u32)r0 == (u32)r5) {
                            r0 = (u32)sp + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                            }
                            r3 = r8 + r10;
                            r0 = *(u32*)((u8*)r3 + 0x4);
                            r26 = r9 + r0;
                            goto L_800FB748;
                        }
                        if ((u32)r0 < (u32)0x0) {
                            r7 = r3 + 0x1;

                        } else {
                            r6 = r3;
                        }
                        L_800FB730: ;
                    } while ((u32)r7 < (u32)r6);
                }
                r9 = *(u32*)((u8*)r9 + 0x8);
        }
        r26 = 0x0;
    }
    L_800FB748: ;
    if ((u32)r26 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x68;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (0x4330 << 16);
    /* xoris r0, r27, 0x8000 */;
    *(u32*)(sp + 0x14) = r0;
    /* xoris r3, r28, 0x8000 */;
    f3 = *(f32*)lbl_8047CD08;
    r6 = 0x1;
    r5 = -0x1;
    f2 = *(f64*)lbl_8047CD10;
    r7 = 0x0;
    f0 = *(f64*)(sp + 0x10);
    r0 = 0x3;
    f1 = f0 - f2;
    r8 = *(u32*)&lbl_80478B08;
    f0 = *(f64*)(sp + 0x18);
    *(u8*)((u8*)r31 + 0x0) = r6;
    f0 = f0 - f2;
    *(f32*)((u8*)r31 + 0x60) = f3;
    *(f32*)((u8*)r31 + 0x64) = f3;
    *(u32*)((u8*)r31 + 0x24) = r5;
    *(u32*)((u8*)r31 + 0x28) = r26;
    *(u32*)((u8*)r31 + 0x2C) = r26;
    *(u32*)((u8*)r31 + 0x30) = r26;
    r3 = *(u8*)((u8*)r3 + 0x3);
    *(u16*)((u8*)r31 + 0x20) = r3;
    *(u32*)((u8*)r31 + 0x1C) = r30;
    *(f32*)((u8*)r31 + 0x4) = f1;
    *(f32*)((u8*)r31 + 0x8) = f0;
    *(u16*)((u8*)r31 + 0x18) = r7;
    *(u16*)((u8*)r31 + 0x1A) = r7;
    *(u8*)((u8*)r31 + 0x44) = r0;
    *(u32*)((u8*)r31 + 0x24) = r29;
    *(u8*)((u8*)r31 + 0x2) = r6;
    r0 = *(u16*)((u8*)r8 + 0x4);
    r4 = r3 & 0xFFFF;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r8 + 0x24);
            r3 = r0 + r7;
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u8*)((u8*)r3 + 0x2);
                *(u8*)((u8*)r31 + 0x22) = r0;
                r3 = *(u8*)((u8*)r3 + 0x3);
                *(u8*)((u8*)r31 + 0x23) = r3;
                if ((u32)r4 == (u32)0x0) {
                    r0 = 0xb;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }

                if ((u32)r4 == (u32)0x1 && (u32)r4 != (u32)0x1) {

                    r0 = 0x6;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CD28;
                *(u32*)(sp + 0x18) = r0;
                f3 = *(f64*)lbl_8047CD20;
                f1 = *(f64*)(sp + 0x18);
                f0 = *(f64*)lbl_8047CD18;
                f1 = f1 - f2;
                f0 = f3 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x10) = f0;
                r0 = (s8)r0;
                *(u8*)((u8*)r31 + 0x42) = r0;
                break;
            }
            r7 = r7 + 0x8;
        } while (--ctr != 0);
    }
    r4 = *(u8*)((u8*)r31 + 0x44);
    r3 = r31;
    r5 = 0x0;
    r6 = 0x0;
    fn_800FC7E0();

    return;
}

/* 0x800FB8C8 | 0x26C */
void fn_800FB8C8(void) {
    extern u8 lbl_80402418[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern void fn_800FA444();
    extern void fn_800FC7E0();
    extern u8 lbl_80478B08;
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    r30 = r8;
    r3 = r30;
    fn_800FA444();
    r4 = (u32)r3 >> 16;
    r3 = (u32)lbl_80402418;
    r0 = (s16)r27;
    r4 = (s16)r4;
    r31 = (u32)lbl_80402418;
    r0 = r0 - r4;
    r25 = r25 + r0;
    if ((u32)r30 == (u32)0x0) {
        r24 = 0x0;

    } else {
        r3 = *(u32*)&lbl_80478B08;
        r4 = (u32)r30 >> 20;
        r5 = r30 & 0xFFFFF;
        r9 = *(u32*)((u8*)r3 + 0x8);
        while ((u32)r9 != (u32)0x0) {
                r0 = *(u16*)((u8*)r9 + 0x0);
                if ((u32)r0 == (u32)r4) {
                    r6 = *(u16*)((u8*)r9 + 0x4);
                    r8 = r9 + 0x10;
                    r7 = 0x0;
                    goto L_800FB99C;
                    do {
                        r0 = r7 + r6;
                        r3 = (u32)r0 >> 1;
                        r10 = r3 << 3;
                        r0 = *(u32*)(r8 + r10);
                        if ((u32)r0 == (u32)r5) {
                            r0 = (u32)sp + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                            }
                            r3 = r8 + r10;
                            r0 = *(u32*)((u8*)r3 + 0x4);
                            r24 = r9 + r0;
                            goto L_800FB9B4;
                        }
                        if ((u32)r0 < (u32)0x0) {
                            r7 = r3 + 0x1;

                        } else {
                            r6 = r3;
                        }
                        L_800FB99C: ;
                    } while ((u32)r7 < (u32)r6);
                }
                r9 = *(u32*)((u8*)r9 + 0x8);
        }
        r24 = 0x0;
    }
    L_800FB9B4: ;
    if ((u32)r24 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x68;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (0x4330 << 16);
    /* xoris r0, r25, 0x8000 */;
    /* xoris r3, r26, 0x8000 */;
    *(u32*)(sp + 0x14) = r0;
    f3 = *(f32*)lbl_8047CD08;
    r6 = 0x1;
    r5 = -0x1;
    f2 = *(f64*)lbl_8047CD10;
    r0 = 0x3;
    f0 = *(f64*)(sp + 0x10);
    r7 = 0x0;
    f1 = f0 - f2;
    r8 = *(u32*)&lbl_80478B08;
    f0 = *(f64*)(sp + 0x18);
    *(u8*)((u8*)r31 + 0x0) = r6;
    f0 = f0 - f2;
    *(f32*)((u8*)r31 + 0x60) = f3;
    *(f32*)((u8*)r31 + 0x64) = f3;
    *(u32*)((u8*)r31 + 0x24) = r5;
    *(u32*)((u8*)r31 + 0x28) = r24;
    *(u32*)((u8*)r31 + 0x2C) = r24;
    *(u32*)((u8*)r31 + 0x30) = r24;
    r3 = *(u8*)((u8*)r3 + 0x3);
    *(u16*)((u8*)r31 + 0x20) = r3;
    *(u32*)((u8*)r31 + 0x1C) = r30;
    *(f32*)((u8*)r31 + 0x4) = f1;
    *(f32*)((u8*)r31 + 0x8) = f0;
    *(u16*)((u8*)r31 + 0x18) = r27;
    *(u16*)((u8*)r31 + 0x1A) = r28;
    *(u8*)((u8*)r31 + 0x44) = r0;
    *(u32*)((u8*)r31 + 0x24) = r29;
    *(u8*)((u8*)r31 + 0x2) = r6;
    r0 = *(u16*)((u8*)r8 + 0x4);
    r4 = r3 & 0xFFFF;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r8 + 0x24);
            r3 = r0 + r7;
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u8*)((u8*)r3 + 0x2);
                *(u8*)((u8*)r31 + 0x22) = r0;
                r3 = *(u8*)((u8*)r3 + 0x3);
                *(u8*)((u8*)r31 + 0x23) = r3;
                if ((u32)r4 == (u32)0x0) {
                    r0 = 0xb;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }

                if ((u32)r4 == (u32)0x1 && (u32)r4 != (u32)0x1) {

                    r0 = 0x6;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CD28;
                *(u32*)(sp + 0x18) = r0;
                f3 = *(f64*)lbl_8047CD20;
                f1 = *(f64*)(sp + 0x18);
                f0 = *(f64*)lbl_8047CD18;
                f1 = f1 - f2;
                f0 = f3 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x10) = f0;
                r0 = (s8)r0;
                *(u8*)((u8*)r31 + 0x42) = r0;
                break;
            }
            r7 = r7 + 0x8;
        } while (--ctr != 0);
    }
    r4 = *(u8*)((u8*)r31 + 0x44);
    r3 = r31;
    r5 = 0x0;
    r6 = 0x0;
    fn_800FC7E0();

    return;
}

/* 0x800FBB34 | 0x254 */
void fn_800FBB34(void) {
    extern u8 lbl_80402418[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern void fn_800FC7E0();
    extern u8 lbl_80478B08;
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    r30 = r8;
    r3 = (u32)lbl_80402418;
    r0 = (u32)lbl_80402418;
    r31 = r0;
    if ((u32)r30 == (u32)0x0) {
        r24 = 0x0;

    } else {
        r3 = *(u32*)&lbl_80478B08;
        r4 = (u32)r30 >> 20;
        r5 = r30 & 0xFFFFF;
        r9 = *(u32*)((u8*)r3 + 0x8);
        while ((u32)r9 != (u32)0x0) {
                r0 = *(u16*)((u8*)r9 + 0x0);
                if ((u32)r0 == (u32)r4) {
                    r6 = *(u16*)((u8*)r9 + 0x4);
                    r8 = r9 + 0x10;
                    r7 = 0x0;
                    goto L_800FBBF0;
                    do {
                        r0 = r7 + r6;
                        r3 = (u32)r0 >> 1;
                        r10 = r3 << 3;
                        r0 = *(u32*)(r8 + r10);
                        if ((u32)r0 == (u32)r5) {
                            r0 = (u32)sp + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                            }
                            r3 = r8 + r10;
                            r0 = *(u32*)((u8*)r3 + 0x4);
                            r24 = r9 + r0;
                            goto L_800FBC08;
                        }
                        if ((u32)r0 < (u32)0x0) {
                            r7 = r3 + 0x1;

                        } else {
                            r6 = r3;
                        }
                        L_800FBBF0: ;
                    } while ((u32)r7 < (u32)r6);
                }
                r9 = *(u32*)((u8*)r9 + 0x8);
        }
        r24 = 0x0;
    }
    L_800FBC08: ;
    if ((u32)r24 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x68;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (0x4330 << 16);
    /* xoris r0, r25, 0x8000 */;
    /* xoris r3, r26, 0x8000 */;
    *(u32*)(sp + 0x14) = r0;
    f3 = *(f32*)lbl_8047CD08;
    r6 = 0x1;
    r5 = -0x1;
    f2 = *(f64*)lbl_8047CD10;
    r0 = 0x3;
    f0 = *(f64*)(sp + 0x10);
    r7 = 0x0;
    f1 = f0 - f2;
    r8 = *(u32*)&lbl_80478B08;
    f0 = *(f64*)(sp + 0x18);
    *(u8*)((u8*)r31 + 0x0) = r6;
    f0 = f0 - f2;
    *(f32*)((u8*)r31 + 0x60) = f3;
    *(f32*)((u8*)r31 + 0x64) = f3;
    *(u32*)((u8*)r31 + 0x24) = r5;
    *(u32*)((u8*)r31 + 0x28) = r24;
    *(u32*)((u8*)r31 + 0x2C) = r24;
    *(u32*)((u8*)r31 + 0x30) = r24;
    r3 = *(u8*)((u8*)r3 + 0x3);
    *(u16*)((u8*)r31 + 0x20) = r3;
    *(u32*)((u8*)r31 + 0x1C) = r30;
    *(f32*)((u8*)r31 + 0x4) = f1;
    *(f32*)((u8*)r31 + 0x8) = f0;
    *(u16*)((u8*)r31 + 0x18) = r27;
    *(u16*)((u8*)r31 + 0x1A) = r28;
    *(u8*)((u8*)r31 + 0x44) = r0;
    *(u32*)((u8*)r31 + 0x24) = r29;
    *(u8*)((u8*)r31 + 0x2) = r6;
    r0 = *(u16*)((u8*)r8 + 0x4);
    r4 = r3 & 0xFFFF;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r8 + 0x24);
            r3 = r0 + r7;
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u8*)((u8*)r3 + 0x2);
                *(u8*)((u8*)r31 + 0x22) = r0;
                r3 = *(u8*)((u8*)r3 + 0x3);
                *(u8*)((u8*)r31 + 0x23) = r3;
                if ((u32)r4 == (u32)0x0) {
                    r0 = 0xb;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }

                if ((u32)r4 == (u32)0x1 && (u32)r4 != (u32)0x1) {

                    r0 = 0x6;
                    *(u8*)((u8*)r31 + 0x42) = r0;
                    break;
                }
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CD28;
                *(u32*)(sp + 0x18) = r0;
                f3 = *(f64*)lbl_8047CD20;
                f1 = *(f64*)(sp + 0x18);
                f0 = *(f64*)lbl_8047CD18;
                f1 = f1 - f2;
                f0 = f3 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x10) = f0;
                r0 = (s8)r0;
                *(u8*)((u8*)r31 + 0x42) = r0;
                break;
            }
            r7 = r7 + 0x8;
        } while (--ctr != 0);
    }
    r4 = *(u8*)((u8*)r31 + 0x44);
    r3 = r31;
    r5 = 0x0;
    r6 = 0x0;
    fn_800FC7E0();

    return;
}

/* 0x800FBD88 | 0xF4 */
void fn_800FBD88(void) {
    extern void fn_801669BC();
    extern u8 lbl_80478B08;
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r7 = *(u32*)&lbl_80478B08;
    r5 = 0x0;
    r6 = *(u16*)((u8*)r7 + 0x0);
    r31 = 0x0;
    for (r5 = 0x0; (s32)(r5 & 0xFF) < (s32)r6; r5 = r5 + 0x1) {
        r0 = r5 & 0xFF;
        r4 = *(u32*)((u8*)r7 + 0x20);
        r0 = r0 * 0x68;
        r31 = r4 + r0;
        r0 = *(u8*)((u8*)r31 + 0x0);
        if ((u32)r0 == (u32)0x0) { r31 = 0x0; continue; }
        r0 = *(u32*)((u8*)r31 + 0x1C);
        if ((u32)r0 != (u32)r3) { r31 = 0x0; continue; }
        break;
    }
    if ((u32)r31 == (u32)0x0) return;
    r0 = *(u8*)((u8*)r31 + 0x3);
    r3 = 0x0;
    switch ((s32)r0) {
        case 0:
            break;
        case 1:
            r3 = 0x57;
            break;
        case 2:
            r3 = 0x58;
            break;
        case 3:
            r3 = 0x59;
            break;
        case 4:
            r3 = 0x497;
            break;
        case 5:
            r3 = 0x498;
            break;
        default:
            break;
    }
    if ((u32)r3 != (u32)0x0) {
        fn_801669BC();
    }
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;

    return;
}

/* 0x800FBE7C | 0x94 */
void fn_800FBE7C(void) {
    extern void fn_800FC7E0();
    extern u8 lbl_80478B08;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r10 = r4;
    r6 = r5;
    r9 = *(u32*)&lbl_80478B08;
    r5 = 0x0;
    r8 = *(u16*)((u8*)r9 + 0x0);
    goto L_800FBED0;
    do {
        r0 = r5 & 0xFF;
        r4 = *(u32*)((u8*)r9 + 0x20);
        r0 = r0 * 0x68;
        r5 = r5 + 0x1;
        r7 = r4 + r0;
        r0 = *(u8*)((u8*)r7 + 0x0);
        if ((u32)r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)r7 + 0x1C);
            if ((u32)r0 == (u32)r3) {
                goto L_800FBEE0;
            }
            }
        L_800FBED0: ;
        r0 = r5 & 0xFF;
    } while ((s32)r0 < (s32)r8);
    r7 = 0x0;
    L_800FBEE0: ;
    if ((u32)r7 == (u32)0x0) {
        r3 = -0x1;
    } else {

        r4 = *(u8*)((u8*)r7 + 0x44);
        r3 = r7;
        r5 = r10;
        fn_800FC7E0();
    }
    return;
}

/* 0x64 | fn_800FBF10 | single_call_straight */
void fn_800FBF10(void) {
    fn_800EF504();
}

/* 0x800FBF74 | 0x25C */
void fn_800FBF74(void) {
    extern u8 lbl_80271730[];
    extern u8 lbl_80271754[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    extern u8 lbl_80478B08;
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    r4 = *(u32*)&lbl_80478B08;
    r6 = 0x0;
    r3 = 0x0;
    r5 = *(u16*)((u8*)r4 + 0x0);
    ctr_fn = (void(*)(void))r5;
    if ((s32)r5 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r4 + 0x20);
            r31 = r0 + r3;
            r0 = *(u8*)((u8*)r31 + 0x0);
            if ((u32)r0 == (u32)0x0) break;
            r3 = r3 + 0x68;
            r6 = r6 + 0x1;
        } while (--ctr != 0);
    }
    if ((s32)r6 == (s32)r5) {
        r3 = (u32)lbl_80271730;
        r4 = r28;
        r3 = (u32)lbl_80271730;
        /* crclr cr1eq */;
        ((void(*)(void))fn_800DD970)();
        r3 = -0x1;
        return;
    }
    if ((u32)r28 == (u32)0x0) {
        r27 = 0x0;

    } else {
        r9 = *(u32*)((u8*)r4 + 0x8);
        r3 = (u32)r28 >> 20;
        r4 = r28 & 0xFFFFF;
        while ((u32)r9 != (u32)0x0) {
                r0 = *(u16*)((u8*)r9 + 0x0);
                if ((u32)r0 == (u32)r3) {
                    r6 = *(u16*)((u8*)r9 + 0x4);
                    r8 = r9 + 0x10;
                    r7 = 0x0;
                    goto L_800FC074;
                    do {
                        r0 = r7 + r6;
                        r5 = (u32)r0 >> 1;
                        r10 = r5 << 3;
                        r0 = *(u32*)(r8 + r10);
                        if ((u32)r0 == (u32)r4) {
                            r0 = (u32)sp + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                            }
                            r3 = r8 + r10;
                            r0 = *(u32*)((u8*)r3 + 0x4);
                            r27 = r9 + r0;
                            goto L_800FC08C;
                        }
                        if ((u32)r0 < (u32)0x0) {
                            r7 = r5 + 0x1;

                        } else {
                            r6 = r5;
                        }
                        L_800FC074: ;
                    } while ((u32)r7 < (u32)r6);
                }
                r9 = *(u32*)((u8*)r9 + 0x8);
        }
        r27 = 0x0;
    }
    L_800FC08C: ;
    if ((u32)r27 == (u32)0x0) {
        r3 = (u32)lbl_80271754;
        r4 = r28;
        r3 = (u32)lbl_80271754;
        /* crclr cr1eq */;
        ((void(*)(void))fn_800DD970)();
        r3 = -0x1;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x68;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = 0x1;
    f0 = *(f32*)lbl_8047CD08;
    *(u8*)((u8*)r31 + 0x0) = r0;
    r0 = -0x1;
    r5 = 0x0;
    *(f32*)((u8*)r31 + 0x60) = f0;
    *(f32*)((u8*)r31 + 0x64) = f0;
    *(u32*)((u8*)r31 + 0x24) = r0;
    *(u32*)((u8*)r31 + 0x28) = r27;
    *(u32*)((u8*)r31 + 0x2C) = r27;
    *(u32*)((u8*)r31 + 0x30) = r27;
    r0 = *(u8*)((u8*)r3 + 0x3);
    *(u16*)((u8*)r31 + 0x20) = r0;
    *(u32*)((u8*)r31 + 0x1C) = r28;
    *(u8*)((u8*)r31 + 0x44) = r29;
    *(u8*)((u8*)r31 + 0x3) = r30;
    r6 = *(u32*)&lbl_80478B08;
    r0 = *(u16*)((u8*)r6 + 0x4);
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) { r3 = 0x0; return; }
    do {
        r3 = *(u32*)((u8*)r6 + 0x24);
        r0 = *(u16*)((u8*)r31 + 0x20);
        r4 = r3 + r5;
        r3 = *(u16*)((u8*)r4 + 0x0);
        if ((u32)r3 == (u32)r0) {
            r0 = *(u8*)((u8*)r4 + 0x2);
            *(u8*)((u8*)r31 + 0x22) = r0;
            r0 = *(u8*)((u8*)r4 + 0x3);
            *(u8*)((u8*)r31 + 0x23) = r0;
            r0 = *(u16*)((u8*)r31 + 0x20);
            if ((u32)r0 == (u32)0x0) {
                r0 = 0xb;
                *(u8*)((u8*)r31 + 0x42) = r0;
                r3 = 0x0;
                return;
            }

            if ((u32)r0 == (u32)0x1 && (u32)r0 != (u32)0x1) {

                r0 = 0x6;
                *(u8*)((u8*)r31 + 0x42) = r0;
                r3 = 0x0;
                return;
            }
            r3 = *(u8*)((u8*)r31 + 0x23);
            r0 = (0x4330 << 16);
            *(u32*)(sp + 0x10) = r0;
            f2 = *(f64*)lbl_8047CD28;
            f3 = *(f64*)lbl_8047CD20;
            f1 = *(f64*)(sp + 0x10);
            f0 = *(f64*)lbl_8047CD18;
            f1 = f1 - f2;
            f0 = f3 * f1 + f0;
            f0 = (f64)(s32)f0;
            *(f64*)(sp + 0x18) = f0;
            r0 = (s8)r0;
            *(u8*)((u8*)r31 + 0x42) = r0;
            r3 = 0x0;
            return;
        }
        r5 = r5 + 0x8;
    } while (--ctr != 0);

    r3 = 0x0;

    return;
}

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
void fn_800FC2A8(void) {
    extern u8 lbl_80478B08;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r6 = *(u32*)&lbl_80478B08;
    r9 = 0x0;
    r5 = 0x0;
    r7 = *(u16*)((u8*)r6 + 0x4);
    ctr_fn = (void(*)(void))r7;
    if ((s32)r7 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r6 + 0x24);
            r8 = r0 + r5;
            r0 = *(u32*)((u8*)r8 + 0x4);
            if ((u32)r0 != (u32)0x0) {
                r4 = *(u16*)((u8*)r8 + 0x0);
                r0 = *(u16*)((u8*)r3 + 0x0);
                if ((u32)r4 == (u32)r0) break;
            }
            r5 = r5 + 0x8;
            r9 = r9 + 0x1;
        } while (--ctr != 0);
    }
    if ((s32)r9 != (s32)r7) {
        r5 = *(u32*)((u8*)r8 + 0x4);
        r0 = r3 + 0x8;
        while ((u32)r5 != (u32)0x0) {
                if ((u32)r5 == (u32)r0) {
                    r4 = *(u32*)((u8*)r5 + 0xC);
                    if ((u32)r4 == (u32)0x0) {
                        r0 = *(u32*)((u8*)r5 + 0x8);
                        if ((u32)r0 == (u32)0x0) {
                            r4 = (0x1 << 16);
                            r0 = 0x0;
                            /* subi r4, r4, 0x1 */;
                            *(u16*)((u8*)r8 + 0x0) = r4;
                            *(u32*)((u8*)r8 + 0x4) = r0;
                            break;
                        }
                        }
                    if ((u32)r4 != (u32)0x0) {
                        r0 = *(u32*)((u8*)r5 + 0x8);
                        *(u32*)((u8*)r4 + 0x8) = r0;
                    } else {

                        r0 = *(u32*)((u8*)r5 + 0x8);
                        *(u32*)((u8*)r8 + 0x4) = r0;
                    }
                    r4 = *(u32*)((u8*)r5 + 0x8);
                    if ((u32)r4 == (u32)0x0) break;
                    r0 = *(u32*)((u8*)r5 + 0xC);
                    *(u32*)((u8*)r4 + 0xC) = r0;
                    break;
                }
                r5 = *(u32*)((u8*)r5 + 0x8);
        }
    }
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)0x0) {
        r3 = r3 + r0;
        /* b fn_800FC2A8 */;
    }
    r3 = 0x0;
    return;
}

/* 0x800FC39C | 0x17C */
void fn_800FC39C(void) {
    extern u8 lbl_8027177C[];
    extern u8 lbl_80478B08;
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r29 = r3;
    r31 = r29;
    while (1) {
        r5 = *(u16*)((u8*)r31 + 0x0);
        if ((u32)r5 == (u32)0xffff) {
            r3 = 0x0;
            return;
        }
        r4 = *(u32*)&lbl_80478B08;
        r7 = 0x0;
        r3 = 0x0;
        r6 = *(u16*)((u8*)r4 + 0x4);
        ctr_fn = (void(*)(void))r6;
        if ((s32)r6 > (s32)0x0) {
            do {
                r0 = *(u32*)((u8*)r4 + 0x24);
                r30 = r0 + r3;
                r0 = *(u32*)((u8*)r30 + 0x4);
                if ((u32)r0 != (u32)0x0) {
                    r0 = *(u16*)((u8*)r30 + 0x0);
                    if ((u32)r0 == (u32)r5) break;
                }
                r3 = r3 + 0x8;
                r7 = r7 + 0x1;
            } while (--ctr != 0);
        }
        if ((s32)r7 == (s32)r6) {
            r7 = 0x0;
            r3 = 0x0;
            ctr_fn = (void(*)(void))r6;
            if ((s32)r6 > (s32)0x0) {
                do {
                    r0 = *(u32*)((u8*)r4 + 0x24);
                    r30 = r0 + r3;
                    r0 = *(u32*)((u8*)r30 + 0x4);
                    if ((u32)r0 == (u32)0x0) {
                        r4 = *(u32*)((u8*)r31 + 0x0);
                        r5 = r31 + 0x8;
                        r3 = *(u32*)((u8*)r31 + 0x4);
                        r0 = 0x0;
                        *(u32*)((u8*)r30 + 0x0) = r4;
                        *(u32*)((u8*)r30 + 0x4) = r3;
                        *(u32*)((u8*)r30 + 0x4) = r5;
                        *(u32*)((u8*)r5 + 0x8) = r0;
                        *(u32*)((u8*)r5 + 0xC) = r0;
                        break;
                    }
                    r3 = r3 + 0x8;
                    r7 = r7 + 0x1;
                } while (--ctr != 0);
            }
            r3 = *(u32*)&lbl_80478B08;
            r0 = *(u16*)((u8*)r3 + 0x4);
            if ((s32)r7 == (s32)r0) {
                r3 = (u32)lbl_8027177C;
                r4 = *(u16*)((u8*)r31 + 0x0);
                r3 = (u32)lbl_8027177C;
                /* crclr cr1eq */;
                ((void(*)(void))fn_800DD970)();

            } else {
                r3 = r31 + 0x8;
                r4 = *(u32*)((u8*)r30 + 0x4);
                while (1) {
                    if ((u32)r4 == (u32)r3) {
                        r3 = 0x0;
                        return;
                    }
                    r0 = *(u32*)((u8*)r4 + 0x8);
                    if ((u32)r0 == (u32)0x0) {
                        *(u32*)((u8*)r4 + 0x8) = r3;
                        r0 = 0x0;
                        *(u32*)((u8*)r3 + 0x8) = r0;
                        *(u32*)((u8*)r3 + 0xC) = r4;
                        break;
                    }
                    r4 = r0;
                }
                }
            }

        r0 = *(u32*)((u8*)r31 + 0x4);
        if ((u32)r0 == (u32)0x0) { r3 = r29; return; }
        r31 = r31 + r0;
    }

    r3 = r29;

    return;
}

/* 0x800FC518 | 0x10 -- store r3 to lbl_80478B08->0x28, return 0 */
extern u32 lbl_80478B08;
u32 fn_800FC518(u32 val) {
    *(u32*)((u8*)lbl_80478B08 + 0x28) = val;
    return 0;
}

/* 0x800FC528 | 0x2B8 */
void fn_800FC528(void) {
    extern u8 lbl_802717B4[];
    extern u8 lbl_804024E8[];
    extern u8 lbl_8047CD08[];
    extern void fn_800E27B0();
    extern void fn_800E3534();
    extern void fn_800EF5FC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    *(f64*)(sp + 0x30) = f31;
    /* psq_st f31, 0x38((u32)sp), 0, qr0 */;
    r29 = r3;
    r30 = r4;
    r3 = (u32)lbl_804024E8;
    r4 = 0x0;
    r3 = (u32)lbl_804024E8;
    r5 = 0x2c;
    memset((void*)r3, (int)r4, (u32)r5);
    r27 = r29 & 0xFFFF;
    r3 = r27 * 0x68;
    fn_800E3534();
    r4 = *(u32*)&lbl_80478B08;
    *(u16*)((u8*)r4 + 0x2) = r3;
    r3 = *(u32*)&lbl_80478B08;
    r3 = *(u16*)((u8*)r3 + 0x2);
    if ((u32)r3 == (u32)0x0) {
        r3 = (u32)lbl_802717B4;
        r3 = (u32)lbl_802717B4;
        /* crclr cr1eq */;
        ((void(*)(void))fn_800DD970)();
        r3 = -0x1;

    } else {
        fn_800E27B0();
        r4 = *(u32*)&lbl_80478B08;
        r31 = r30 & 0xFFFF;
        *(u32*)((u8*)r4 + 0x20) = r3;
        r3 = r31 << 3;
        fn_800E3534();
        r4 = *(u32*)&lbl_80478B08;
        *(u16*)((u8*)r4 + 0x6) = r3;
        r3 = *(u32*)&lbl_80478B08;
        r3 = *(u16*)((u8*)r3 + 0x6);
        if ((u32)r3 == (u32)0x0) {
            r3 = (u32)lbl_802717B4;
            r3 = (u32)lbl_802717B4;
            /* crclr cr1eq */;
            ((void(*)(void))fn_800DD970)();
            r3 = -0x1;

        } else {
            fn_800E27B0();
            r4 = *(u32*)&lbl_80478B08;
            r25 = 0x0;
            r28 = 0x0;
            *(u32*)((u8*)r4 + 0x24) = r3;
            f31 = *(f32*)lbl_8047CD08;
            while ((s32)r25 < (s32)r27) {

                r3 = *(u32*)&lbl_80478B08;
                r4 = 0x0;
                r5 = 0x68;
                r0 = *(u32*)((u8*)r3 + 0x20);
                r26 = r0 + r28;
                r3 = r26;
                memset((void*)r3, (int)r4, (u32)r5);
                *(f32*)((u8*)r26 + 0x60) = f31;
                r28 = r28 + 0x68;
                r25 = r25 + 0x1;
                *(f32*)((u8*)r26 + 0x64) = f31;

            }
            r4 = *(u32*)&lbl_80478B08;
            r3 = 0x0;
            *(u16*)((u8*)r4 + 0x0) = r29;
            if ((s32)r31 > (s32)0x0) {
                /* subi r5, r31, 0x8 */;
                if ((s32)r31 > (s32)0x8) {
                    r7 = r3;
                    r0 = r5 + 0x7;
                    r4 = (0x1 << 16);
                    r0 = (u32)r0 >> 3;
                    /* subi r6, r4, 0x1 */;
                    ctr_fn = (void(*)(void))r0;
                    if ((s32)r5 > (s32)0x0) {
                        do {
                            r4 = *(u32*)&lbl_80478B08;
                            r5 = 0x0;
                            r28 = r7 + 0x8;
                            r27 = r7 + 0x10;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r12 = r7 + 0x18;
                            r11 = r7 + 0x20;
                            r10 = r7 + 0x28;
                            r4 = r0 + r7;
                            r9 = r7 + 0x30;
                            *(u16*)((u8*)r4 + 0x0) = r6;
                            r8 = r7 + 0x38;
                            r7 = r7 + 0x40;
                            r3 = r3 + 0x8;
                            *(u32*)((u8*)r4 + 0x4) = r5;
                            r4 = *(u32*)&lbl_80478B08;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r28 = r0 + r28;
                            *(u16*)((u8*)r28 + 0x0) = r6;
                            *(u32*)((u8*)r28 + 0x4) = r5;
                            r4 = *(u32*)&lbl_80478B08;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r27 = r0 + r27;
                            *(u16*)((u8*)r27 + 0x0) = r6;
                            *(u32*)((u8*)r27 + 0x4) = r5;
                            r4 = *(u32*)&lbl_80478B08;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r12 = r0 + r12;
                            *(u16*)((u8*)r12 + 0x0) = r6;
                            *(u32*)((u8*)r12 + 0x4) = r5;
                            r4 = *(u32*)&lbl_80478B08;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r11 = r0 + r11;
                            *(u16*)((u8*)r11 + 0x0) = r6;
                            *(u32*)((u8*)r11 + 0x4) = r5;
                            r4 = *(u32*)&lbl_80478B08;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r10 = r0 + r10;
                            *(u16*)((u8*)r10 + 0x0) = r6;
                            *(u32*)((u8*)r10 + 0x4) = r5;
                            r4 = *(u32*)&lbl_80478B08;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r9 = r0 + r9;
                            *(u16*)((u8*)r9 + 0x0) = r6;
                            *(u32*)((u8*)r9 + 0x4) = r5;
                            r4 = *(u32*)&lbl_80478B08;
                            r0 = *(u32*)((u8*)r4 + 0x24);
                            r8 = r0 + r8;
                            *(u16*)((u8*)r8 + 0x0) = r6;
                            *(u32*)((u8*)r8 + 0x4) = r5;
                        } while (--ctr != 0);
                }
                }
                r6 = r3 << 3;
                r4 = (0x1 << 16);
                r0 = r31 - r3;
                /* subi r5, r4, 0x1 */;
                r4 = 0x0;
                ctr_fn = (void(*)(void))r0;
                if ((s32)r3 < (s32)r31) {
                    do {
                        r3 = *(u32*)&lbl_80478B08;
                        r0 = *(u32*)((u8*)r3 + 0x24);
                        r3 = r0 + r6;
                        r6 = r6 + 0x8;
                        *(u16*)((u8*)r3 + 0x0) = r5;
                        *(u32*)((u8*)r3 + 0x4) = r4;
                    } while (--ctr != 0);
            }
            }
            r6 = *(u32*)&lbl_80478B08;
            r3 = 0x200;
            r4 = 0x200;
            r5 = 0x40;
            *(u16*)((u8*)r6 + 0x4) = r30;
            r6 = 0x0;
            r7 = 0x0;
            fn_800EF5FC();
            r7 = *(u32*)&lbl_80478B08;
            r4 = 0x200;
            r5 = 0x40;
            r6 = 0x0;
            *(u32*)((u8*)r7 + 0xC) = r3;
            r3 = 0x200;
            r7 = 0x0;
            fn_800EF5FC();
            r4 = *(u32*)&lbl_80478B08;
            *(u32*)((u8*)r4 + 0x10) = r3;
            r3 = 0x0;
        }
        }

    /* psq_l f31, 0x38((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x30);
    return;
}

/* 0x800FC7E0 | 0xB68 */
void fn_800FC7E0(void) {
    extern u8 lbl_80271700[];
    extern u8 lbl_80314E08[];
    extern u8 lbl_80314F98[];
    extern u8 lbl_80315678[];
    extern u8 lbl_8047AC68[];
    extern u8 lbl_8047CD00[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD28[];
    extern u8 lbl_8047CD30[];
    extern u8 lbl_8047CD3C[];
    extern u8 lbl_8047CD40[];
    extern u8 lbl_8047CD44[];
    extern u8 lbl_8047CD48[];
    extern void fn_800CDBE0();
    extern void fn_800D3088();
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800DBEB4();
    extern void fn_800DBF78();
    extern void fn_800DBFD4();
    extern void fn_800DC04C();
    extern void fn_800DC0D4();
    extern void fn_800DC14C();
    extern void fn_800DC1D4();
    extern void fn_800DC224();
    extern void fn_800EF504();
    extern void fn_800EF548();
    extern void fn_800FD348();
    extern void fn_800FD69C();
    extern void fn_800FDF1C();
    extern void fn_800FE4D4();
    extern void fn_801040F0();
    extern void fn_801669BC();
    extern void fn_80166A28();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    r30 = r4;
    r23 = r5;
    r22 = r6;
    r0 = *(u32*)lbl_8047CD00;
    r27 = 0x0;
    r29 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    if ((u32)r31 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    fn_800FE4D4();
    r3 = (0x8000 << 16);
    r3 = r3 + 0x3;
    fn_800D88DC();
    r3 = 0x4;
    fn_800D888C();
    r3 = (u32)lbl_80314F98;
    r3 = (u32)lbl_80314F98;
    fn_800D7820();
    r4 = *(u32*)&lbl_80478B08;
    r3 = 0x0;
    r0 = *(u8*)((u8*)r4 + 0x1D);
    r0 = (s8)r0;
    r0 = r0 << 2;
    r4 = r4 + r0;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_800D85D4();
    r3 = 0x1;
    fn_800DC1D4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_800DC224();
    r4 = (u32)sp + 0xc;
    r3 = 0x0;
    *(u32*)(sp + 0xC) = r0;
    fn_800DBEB4();
    r3 = 0x0;
    r4 = 0xc;
    fn_800DBF78();
    r3 = 0x0;
    r4 = 0xf;
    r5 = 0xe;
    r6 = 0xa;
    r7 = 0xf;
    fn_800DC0D4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    fn_800DC14C();
    r3 = 0x0;
    r4 = 0x7;
    r5 = 0x4;
    r6 = 0x5;
    r7 = 0x7;
    fn_800DBFD4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    fn_800DC04C();
    r3 = *(u32*)&lbl_80478B08;
    r4 = 0x0;
    r0 = *(u8*)((u8*)r3 + 0x1D);
    r0 = (s8)r0;
    r0 = r0 << 2;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_800EF548();
    r0 = r23 & 0x00000030;
    r4 = *(u32*)&lbl_80478B08;
    *(u32*)((u8*)r4 + 0x14) = r3;
    if ((u32)r0 != (u32)0x0) {
        r0 = 0x1;
        *(u8*)((u8*)r31 + 0x45) = r0;
    } else {

        r0 = 0x0;
        *(u8*)((u8*)r31 + 0x45) = r0;
    }
    r3 = 0x0;
    r0 = r22 & 0xFF;
    *(u8*)((u8*)r31 + 0x1) = r3;
    r22 = 0x0;
    *(u8*)((u8*)r31 + 0x4B) = r3;
    *(u8*)((u8*)r31 + 0x46) = r3;
    if ((u32)r0 == (u32)0x0) {
        r23 = 0x0;
        goto L_800FCBD0;
        L_800FC990: ;
        r3 = *(u32*)((u8*)r31 + 0x30);
        r4 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r4 == (u32)0x0) {
            r3 = *(u8*)((u8*)r31 + 0x40);
            r0 = (s8)r3;
            if ((s32)r0 == (s32)0x0) {
        r0 = r4 & 0xFFFF;
        if ((u32)r0 == (u32)0x0) {
            r0 = 0x2;
            *(u8*)((u8*)r31 + 0x0) = r0;

        } else {
            if ((u32)r0 == (u32)0xffff) {
                r4 = *(u32*)((u8*)r31 + 0x30);
                r0 = r4 + 0x1;
                *(u32*)((u8*)r31 + 0x30) = r0;
                r3 = *(u32*)&lbl_80478B08;
                r4 = *(u8*)((u8*)r4 + 0x0);
                r3 = *(u32*)((u8*)r3 + 0x28);
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;

                } else {
                    r0 = *(u8*)((u8*)r31 + 0x1);
                    if ((u32)r0 == (u32)0x0) {
                        r0 = r4 << 3;
                        r0 = *(u8*)(r3 + r0);
                        /* extrwi r0, r0, 1, 27 */;
                    } else {

                        r0 = r4 << 3;
                        r0 = *(u8*)(r3 + r0);
                        /* extrwi r0, r0, 1, 28 */;
                    }
                    if ((u32)r0 == (u32)0x0) {
                        r0 = 0x0;

                    } else {
                        r0 = r4 << 3;
                        r22 = r3 + r0;
                        r12 = *(u32*)((u8*)r22 + 0x4);
                        if ((u32)r12 != (u32)0x0) {
                            r3 = r31;
                            ctr_fn = (void(*)(void))r12;
                            ctr_fn();
                            r0 = *(u8*)((u8*)r22 + 0x0);
                            /* extrwi r0, r0, 2, 24 */;
                            if ((u32)r0 != (u32)0x0 || (u32)r3 == (u32)0x0) {

                                if ((s32)r0 != (s32)0x2) {
                                    if ((s32)r0 < (s32)0x2) {
                                        if ((s32)r0 < (s32)0x1) {

                                        } else {
                                            r21 = r3;

                                        }
                                    } else {
                                            if ((u32)r3 == (u32)0x0) {
                                                r21 = 0x0;

                                            } else {
                                                r4 = *(u32*)&lbl_80478B08;
                                                r5 = (u32)r3 >> 20;
                                                r3 = r3 & 0xFFFFF;
                                                r9 = *(u32*)((u8*)r4 + 0x8);
                                                while ((u32)r9 != (u32)0x0) {
                                                        r0 = *(u16*)((u8*)r9 + 0x0);
                                                        if ((u32)r0 == (u32)r5) {
                                                            r6 = *(u16*)((u8*)r9 + 0x4);
                                                            r8 = r9 + 0x10;
                                                            r7 = 0x0;
                                                            goto L_800FCB2C;
                                                            do {
                                                                r0 = r7 + r6;
                                                                r4 = (u32)r0 >> 1;
                                                                r0 = r4 << 3;
                                                                r10 = r8 + r0;
                                                                r0 = *(u32*)((u8*)r10 + 0x0);
                                                                if ((u32)r0 == (u32)r3) {
                                                                    r0 = *(u32*)((u8*)r10 + 0x4);
                                                                    r21 = r9 + r0;
                                                                    goto L_800FCB44;
                                                                }
                                                                if ((u32)r0 < (u32)r3) {
                                                                    r7 = r4 + 0x1;

                                                                } else {
                                                                    r6 = r4;
                                                                }
                                                                L_800FCB2C: ;
                                                            } while ((u32)r7 < (u32)r6);
                                                        }
                                                        r9 = *(u32*)((u8*)r9 + 0x8);
                                                }
                                                r21 = 0x0;
                                                }
                                            }
                                            }
                                L_800FCB44: ;
                                r5 = *(u8*)((u8*)r31 + 0x40);
                                r3 = (s8)r5;
                                if ((s32)r3 >= (s32)0x3) {
                                    r3 = (u32)lbl_80271700;
                                    r4 = (u32)lbl_80315678;
                                    r3 = (u32)lbl_80271700;
                                    r4 = (u32)lbl_80315678;
                                    /* crclr cr1eq */;
                                    ((void(*)(void))fn_800DD970)();

                                } else {
                                    r4 = *(u32*)((u8*)r31 + 0x30);
                                    r0 = r5 + 0x1;
                                    r3 = r3 << 2;
                                    *(u8*)((u8*)r31 + 0x40) = r0;
                                    r0 = r3 + 0x34;
                                    *(u32*)(r31 + r0) = r4;
                                    *(u32*)((u8*)r31 + 0x30) = r21;
                                }
                                }
                                }

                        r0 = *(u8*)((u8*)r22 + 0x0);
                        /* extrwi r0, r0, 1, 26 */;
                    }
                    }

                r22 = r0;

            } else {
                r0 = *(u8*)((u8*)r31 + 0x4B);
                if ((u32)r0 != (u32)0x2) {
                    r0 = r30 & 0xFF;
                    r22 = 0x1;
                    if ((u32)r0 != (u32)0x0) {
                        r22 = 0x0;
                    }
                    r29 = 0x1;
                }
            }

            r0 = r22 & 0xFF;
            if ((u32)r0 == (u32)0x0) goto L_800FC990;
        }

        r23 = r23 + 0x1;
        L_800FCBD0: ;
        fn_800D3088();
        if ((u32)r23 < (u32)r3) goto L_800FC990;
    }
    r6 = *(u32*)((u8*)r31 + 0x34);
    r3 = 0x1;
    r5 = *(u32*)((u8*)r31 + 0x38);
    r0 = 0x0;
    r4 = *(u32*)((u8*)r31 + 0x3C);
    r25 = (u32)sp + 0x14;
    r26 = *(u32*)((u8*)r31 + 0x30);
    r24 = *(u8*)((u8*)r31 + 0x40);
    *(u8*)((u8*)r31 + 0x1) = r3;
    f0 = *(f32*)((u8*)r31 + 0x4);
    *(f32*)((u8*)r31 + 0xC) = f0;
    f0 = *(f32*)((u8*)r31 + 0x8);
    *(f32*)((u8*)r31 + 0x10) = f0;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    *(u32*)((u8*)r31 + 0x30) = r3;
    *(u8*)((u8*)r31 + 0x40) = r0;
    *(u8*)((u8*)r31 + 0x4B) = r0;
    L_800FCC2C: ;
    r0 = *(u32*)((u8*)r31 + 0x30);
    if ((u32)r0 == (u32)r26) {
        r6 = *(u8*)((u8*)r31 + 0x40);
        r4 = r25;
        r5 = r31;
        r7 = 0x0;
        r6 = (s8)r6;
        ctr_fn = (void(*)(void))r6;
        if ((s32)r6 > (s32)0x0) {
            do {
                r3 = *(u32*)((u8*)r5 + 0x34);
                r0 = *(u32*)((u8*)r4 + 0x0);
                if ((u32)r3 != (u32)r0) break;
                r4 = r4 + 0x4;
                r5 = r5 + 0x4;
                r7 = r7 + 0x1;
            } while (--ctr != 0);
        }
        if ((s32)r7 != (s32)r6) {
        }
        r3 = *(u32*)((u8*)r31 + 0x30);
        r23 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r23 == (u32)0x0) {
            r3 = *(u8*)((u8*)r31 + 0x40);
            r0 = (s8)r3;
            if ((s32)r0 == (s32)0x0) {
        r3 = r23 & 0xFFFF;
        if ((u32)r3 != (u32)0x0) {
            if ((u32)r3 == (u32)0xffff) {
                r4 = *(u32*)((u8*)r31 + 0x30);
                r0 = r4 + 0x1;
                *(u32*)((u8*)r31 + 0x30) = r0;
                r3 = *(u32*)&lbl_80478B08;
                r4 = *(u8*)((u8*)r4 + 0x0);
                r3 = *(u32*)((u8*)r3 + 0x28);
                if ((u32)r3 != (u32)0x0) {
                    r0 = *(u8*)((u8*)r31 + 0x1);
                    if ((u32)r0 == (u32)0x0) {
                        r0 = r4 << 3;
                        r0 = *(u8*)(r3 + r0);
                        /* extrwi r0, r0, 1, 27 */;
                    } else {

                        r0 = r4 << 3;
                        r0 = *(u8*)(r3 + r0);
                        /* extrwi r0, r0, 1, 28 */;
                    }
                    if ((u32)r0 != (u32)0x0) {
                        r0 = r4 << 3;
                        r19 = r3 + r0;
                        r12 = *(u32*)((u8*)r19 + 0x4);
                        if ((u32)r12 != (u32)0x0) {
                            r3 = r31;
                            ctr_fn = (void(*)(void))r12;
                            ctr_fn();
                            r0 = *(u8*)((u8*)r19 + 0x0);
                            /* extrwi r0, r0, 2, 24 */;
                            if ((u32)r0 != (u32)0x0 || (u32)r3 == (u32)0x0) {

                                if ((s32)r0 != (s32)0x2) {
                                    if ((s32)r0 < (s32)0x2) {
                                        if ((s32)r0 < (s32)0x1) {

                                        } else {
                                            r28 = r3;

                                        }
                                    } else {
                                            if ((u32)r3 == (u32)0x0) {
                                                r28 = 0x0;

                                            } else {
                                                r4 = *(u32*)&lbl_80478B08;
                                                r5 = (u32)r3 >> 20;
                                                r3 = r3 & 0xFFFFF;
                                                r9 = *(u32*)((u8*)r4 + 0x8);
                                                while ((u32)r9 != (u32)0x0) {
                                                        r0 = *(u16*)((u8*)r9 + 0x0);
                                                        if ((u32)r0 == (u32)r5) {
                                                            r6 = *(u16*)((u8*)r9 + 0x4);
                                                            r8 = r9 + 0x10;
                                                            r7 = 0x0;
                                                            goto L_800FCE00;
                                                            do {
                                                                r0 = r7 + r6;
                                                                r4 = (u32)r0 >> 1;
                                                                r0 = r4 << 3;
                                                                r10 = r8 + r0;
                                                                r0 = *(u32*)((u8*)r10 + 0x0);
                                                                if ((u32)r0 == (u32)r3) {
                                                                    r0 = *(u32*)((u8*)r10 + 0x4);
                                                                    r28 = r9 + r0;
                                                                    goto L_800FCE18;
                                                                }
                                                                if ((u32)r0 < (u32)r3) {
                                                                    r7 = r4 + 0x1;

                                                                } else {
                                                                    r6 = r4;
                                                                }
                                                                L_800FCE00: ;
                                                            } while ((u32)r7 < (u32)r6);
                                                        }
                                                        r9 = *(u32*)((u8*)r9 + 0x8);
                                                }
                                                r28 = 0x0;
                                                }
                                            }
                                            }
                                L_800FCE18: ;
                                r5 = *(u8*)((u8*)r31 + 0x40);
                                r3 = (s8)r5;
                                if ((s32)r3 >= (s32)0x3) {
                                    r3 = (u32)lbl_80271700;
                                    r4 = (u32)lbl_80315678;
                                    r3 = (u32)lbl_80271700;
                                    r4 = (u32)lbl_80315678;
                                    /* crclr cr1eq */;
                                    ((void(*)(void))fn_800DD970)();

                                } else {
                                    r4 = *(u32*)((u8*)r31 + 0x30);
                                    r0 = r5 + 0x1;
                                    r3 = r3 << 2;
                                    *(u8*)((u8*)r31 + 0x40) = r0;
                                    r0 = r3 + 0x34;
                                    *(u32*)(r31 + r0) = r4;
                                    *(u32*)((u8*)r31 + 0x30) = r28;
                                }
                                }
                                }
                                }
                                }

                r0 = r27 & 0xFF;
                if ((u32)r0 == (u32)0x0) goto L_800FCC2C;
                f2 = *(f32*)((u8*)r31 + 0xC);
                f0 = *(f32*)((u8*)r31 + 0x4);
                if (f2 != f0) goto L_800FCC2C;
                r3 = *(u8*)((u8*)r31 + 0x22);
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x20) = r0;
                /* xoris r0, r3, 0x8000 */;
                f1 = *(f64*)lbl_8047CD10;
                *(u32*)(sp + 0x24) = r0;
                f0 = *(f64*)(sp + 0x20);
                f0 = f0 - f1;
                f0 = f2 + f0;
                *(f32*)((u8*)r31 + 0xC) = f0;
                goto L_800FCC2C;
            }
            r0 = *(u8*)((u8*)r31 + 0x4B);
            if ((u32)r0 == (u32)0x2) goto L_800FCC2C;
            if ((u32)r3 == (u32)0x20) {
                r3 = *(u8*)((u8*)r31 + 0x22);
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x20) = r0;
                r0 = (u32)r3 >> 1;
                f2 = *(f64*)lbl_8047CD10;
                /* xoris r0, r0, 0x8000 */;
                f0 = *(f32*)((u8*)r31 + 0x60);
                *(u32*)(sp + 0x24) = r0;
                f1 = *(f64*)(sp + 0x20);
                f1 = f1 - f2;
                f0 = f1 * f0;
                *(f32*)((u8*)r31 + 0x14) = f0;

            } else {
                r3 = r31;
                r4 = r23;
                r5 = (u32)sp + 0x8;
                fn_800FDF1C();
                r6 = r3;
                if ((u32)r6 == (u32)0x0) {
                    f0 = *(f32*)((u8*)r31 + 0x10);
                    r6 = (0x4330 << 16);
                    f1 = *(f32*)((u8*)r31 + 0xC);
                    r3 = (0x8000 << 16);
                    f0 = (f64)(s32)f0;
                    r0 = *(u8*)((u8*)r31 + 0x22);
                    f1 = (f64)(s32)f1;
                    r4 = *(u8*)((u8*)r31 + 0x23);
                    *(u32*)(sp + 0x34) = r0;
                    r3 = r3 + 0x2;
                    *(f64*)(sp + 0x28) = f0;
                    f6 = *(f64*)lbl_8047CD28;
                    *(f64*)(sp + 0x20) = f1;
                    r0 = r5 + 0x2;
                    r22 = (s16)r0;
                    r0 = (s16)r21;
                    /* xoris r5, r0, 0x8000 */;
                    /* xoris r0, r22, 0x8000 */;
                    f0 = *(f64*)(sp + 0x30);
                    f4 = *(f64*)lbl_8047CD10;
                    f5 = f0 - f6;
                    f0 = *(f64*)(sp + 0x38);
                    f2 = f0 - f4;
                    f3 = *(f32*)((u8*)r31 + 0x60);
                    f1 = *(f32*)((u8*)r31 + 0x64);
                    f0 = *(f64*)(sp + 0x48);
                    f3 = f5 * f3 + f2;
                    *(u32*)(sp + 0x54) = r0;
                    f2 = f0 - f6;
                    f3 = (f64)(s32)f3;
                    f0 = *(f64*)(sp + 0x50);
                    *(f64*)(sp + 0x40) = f3;
                    f0 = f0 - f4;
                    f0 = f2 * f1 + f0;
                    f0 = (f64)(s32)f0;
                    *(f64*)(sp + 0x58) = f0;
                    fn_800D888C();
                    r3 = 0x7;
                    fn_800D6A00();
                    r3 = (u32)lbl_80314E08;
                    r3 = (u32)lbl_80314E08;
                    fn_800D7820();
                    r3 = 0x2;
                    fn_800D67BC();
                    r3 = r21;
                    r4 = r22;
                    fn_800D61E4();
                    r3 = 0x0;
                    r4 = 0xff;
                    r5 = 0xff;
                    r6 = 0xff;
                    r7 = 0xff;
                    fn_800D5CB8();
                    r3 = r19;
                    r4 = r20;
                    fn_800D61E4();
                    r3 = 0x0;
                    r4 = 0xff;
                    r5 = 0xff;
                    r6 = 0xff;
                    r7 = 0xff;
                    fn_800D5CB8();
                    fn_800D6728();
                    r3 = (0x8000 << 16);
                    r3 = r3 + 0x2;
                    fn_800D88DC();
                    r3 = (u32)lbl_80314F98;
                    r3 = (u32)lbl_80314F98;
                    fn_800D7820();
                    r3 = 0x1;
                    fn_800DC1D4();
                    r3 = *(u8*)((u8*)r31 + 0x22);
                    r0 = (0x4330 << 16);
                    *(u32*)(sp + 0x60) = r0;
                    f2 = *(f64*)lbl_8047CD28;
                    f0 = *(f32*)((u8*)r31 + 0x60);
                    f1 = *(f64*)(sp + 0x60);
                    f3 = *(f32*)lbl_8047CD30;
                    f1 = f1 - f2;
                    f0 = f1 * f0;
                    f0 = f3 + f0;
                    *(f32*)((u8*)r31 + 0x14) = f0;

                } else {
                    r3 = r31;
                    r0 = *(u32*)((u8*)r6 + 0x4);
                    r5 = *(u32*)((u8*)r8 + 0x4);
                    r4 = r0 & 0xFFFFFF;
                    r21 = *(u8*)((u8*)r6 + 0x2);
                    r4 = r5 + r4;
                    r0 = (u32)r0 >> 24;
                    r6 = *(u8*)((u8*)r6 + 0x3);
                    r7 = (s8)r0;
                    r5 = r21;
                    r4 = r8 + r4;
                    fn_800FD69C();
                    r3 = (s16)r21;
                    r0 = (0x4330 << 16);
                    /* xoris r3, r3, 0x8000 */;
                    *(u32*)(sp + 0x60) = r0;
                    f2 = *(f64*)lbl_8047CD10;
                    f0 = *(f32*)((u8*)r31 + 0x60);
                    f1 = *(f64*)(sp + 0x60);
                    f1 = f1 - f2;
                    f0 = f1 * f0;
                    *(f32*)((u8*)r31 + 0x14) = f0;
                }
                }

            f1 = *(f32*)((u8*)r31 + 0xC);
            f0 = *(f32*)((u8*)r31 + 0x14);
            f0 = f1 + f0;
            *(f32*)((u8*)r31 + 0xC) = f0;
            r0 = *(u8*)((u8*)r31 + 0x41);
            r0 = (s8)r0;
            if ((s32)r0 == (s32)0x0) {
                r0 = r23 & 0xFFFF;
                if ((u32)r0 == (u32)0x300c) {
                    r27 = 0x1;
                }
                r0 = r23 & 0xFFFF;
                if ((u32)r0 == (u32)0x300d) {
                    r27 = 0x0;
            }
            }
            r0 = *(u8*)((u8*)r31 + 0x4B);
            if ((u32)r0 != (u32)0x1) goto L_800FCC2C;
            r3 = r31;
            fn_800FD348();
            goto L_800FCC2C;
        }
        }
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r29 = 0x0;
    }
    r0 = *(u8*)((u8*)r31 + 0x47);
    r3 = r29 & 0xFF;
    if ((u32)r3 != (u32)r0) {
        r0 = *(u8*)((u8*)r31 + 0x3);
        r3 = 0x0;
        if ((s32)r0 != (s32)0x3) {
            if ((s32)r0 < (s32)0x3) {
                if ((s32)r0 != (s32)0x1) {
                    if ((s32)r0 < (s32)0x1) {
                        if ((s32)r0 < (s32)0x0) {

                        } else {
                            if ((s32)r0 != (s32)0x5) {
                                if ((s32)r0 < (s32)0x5) {
                        r3 = 0x497;

                                } else {
                        r3 = 0x498;
                                    }
                                    }
                            }
                                }

        if ((u32)r3 != (u32)0x0) {
            r0 = r29 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                fn_80166A28();

            } else {
                fn_801669BC();
            }
            }

        *(u8*)((u8*)r31 + 0x47) = r29;
    }
    r0 = *(u8*)((u8*)r31 + 0x46);
    if ((u32)r0 != (u32)0x0) {
        r3 = *(u32*)lbl_8047AC68;
        r0 = (0x4330 << 16);
        *(u32*)(sp + 0x50) = r0;
        /* xoris r0, r3, 0x8000 */;
        f2 = *(f64*)lbl_8047CD10;
        *(u32*)(sp + 0x54) = r0;
        f3 = *(f32*)lbl_8047CD40;
        f1 = *(f64*)(sp + 0x50);
        f0 = *(f32*)lbl_8047CD44;
        f1 = f1 - f2;
        f6 = *(f32*)lbl_8047CD30;
        f5 = *(f32*)((u8*)r31 + 0xC);
        f4 = *(f32*)lbl_8047CD3C;
        f1 = f3 * f1;
        f2 = *(f32*)((u8*)r31 + 0x10);
        f3 = f6 + f5;
        f2 = f4 + f2;
        f1 = f1 / f0;
        f3 = (f64)(s32)f3;
        f0 = (f64)(s32)f2;
        *(f64*)(sp + 0x60) = f3;
        *(f64*)(sp + 0x58) = f0;
        fn_800CDBE0();
        f1 = (f32)f1;
        f0 = *(f32*)lbl_8047CD48;
        f0 = f0 * f1;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x48) = f0;
        r21 = r21 + r0;
        fn_800D3088();
        r5 = *(u32*)lbl_8047AC68;
        r4 = (0x51ec << 16);
        /* subi r0, r4, 0x7ae1 */;
        r5 = r5 + r3;
        r3 = (0x8000 << 16);
        r0 = (s32)((s64)r0 * (s64)r5 >> 32);
        *(u32*)lbl_8047AC68 = r5;
        r0 = (s32)r0 >> 4;
        r4 = (u32)r0 >> 31;
        r0 = r0 + r4;
        r0 = r0 * 0x32;
        r0 = r5 - r0;
        *(u32*)lbl_8047AC68 = r0;
        fn_800D888C();
        r3 = (u32)lbl_80314E08;
        r3 = (u32)lbl_80314E08;
        fn_800D7820();
        r3 = r22;
        r4 = r21;
        r5 = 0x0;
        r6 = 0xb9;
        r7 = 0x0;
        fn_801040F0();
        r3 = (0x8000 << 16);
        fn_800D88DC();
        r3 = (u32)lbl_80314F98;
        r3 = (u32)lbl_80314F98;
        fn_800D7820();
        r3 = 0x1;
        fn_800DC1D4();
    }
    *(u32*)((u8*)r31 + 0x30) = r26;
    *(u8*)((u8*)r31 + 0x40) = r24;
    *(u32*)((u8*)r31 + 0x34) = r0;
    *(u32*)((u8*)r31 + 0x38) = r3;
    *(u32*)((u8*)r31 + 0x3C) = r0;
    r3 = *(u32*)&lbl_80478B08;
    r0 = *(u8*)((u8*)r3 + 0x1D);
    r0 = (s8)r0;
    r0 = r0 << 2;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_800EF504();
    r3 = (0x8000 << 16);
    fn_800D888C();
    r3 = 0x0;

    return;
}
}
}
}
}
}
}
}

/* 0x800FD348 | 0x354 */
void fn_800FD348(void) {
    extern u8 lbl_80314E08[];
    extern u8 lbl_80314F98[];
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD28[];
    extern u8 lbl_8047CD30[];
    extern u8 lbl_8047CD34[];
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800DC1D4();
    extern void fn_800FD69C();
    extern void fn_800FDF1C();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x90) = f31;
    /* psq_st f31, 0x98((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x80) = f30;
    /* psq_st f30, 0x88((u32)sp), 0, qr0 */;
    r28 = r3;
    r3 = *(u8*)((u8*)r28 + 0x58);
    f1 = *(f32*)lbl_8047CD34;
    r0 = r3 + 0x1;
    *(u8*)((u8*)r28 + 0x58) = r0;
    r4 = *(u8*)((u8*)r28 + 0x5B);
    r0 = *(u8*)((u8*)r28 + 0x58);
    r3 = *(u8*)((u8*)r28 + 0x5A);
    r4 = r4 * r0;
    r0 = *(u8*)((u8*)r28 + 0x59);
    f31 = *(f32*)((u8*)r28 + 0xC);
    f30 = *(f32*)((u8*)r28 + 0x10);
    f0 = *(f32*)((u8*)r28 + 0x4C);
    *(f32*)((u8*)r28 + 0xC) = f0;
    r3 = (s32)r4 / (s32)r3;
    f0 = *(f32*)((u8*)r28 + 0x50);
    *(f32*)((u8*)r28 + 0x10) = f0;
    f0 = *(f32*)((u8*)r28 + 0x60);
    f0 = f0 * f1;
    *(f32*)((u8*)r28 + 0x60) = f0;
    f0 = *(f32*)((u8*)r28 + 0x64);
    r31 = r3 & 0xFF;
    r0 = r31 - r0;
    f0 = f0 * f1;
    r30 = r0 & 0xFF;
    *(f32*)((u8*)r28 + 0x64) = f0;
    r29 = *(u32*)((u8*)r28 + 0x54);
    goto L_800FD630;
    do {
        r4 = *(u16*)((u8*)r29 + 0x0);
        r29 = r29 + 0x2;
        if ((u32)r4 == (u32)0xffff) {
            r29 = r29 + 0x1;

        } else {
            if ((u32)r4 == (u32)0x20) {
                r3 = *(u8*)((u8*)r28 + 0x22);
                r0 = (0x4330 << 16);
                *(u32*)(sp + 0x10) = r0;
                r0 = (u32)r3 >> 1;
                f2 = *(f64*)lbl_8047CD10;
                /* xoris r0, r0, 0x8000 */;
                f0 = *(f32*)((u8*)r28 + 0x60);
                *(u32*)(sp + 0x14) = r0;
                f1 = *(f64*)(sp + 0x10);
                f1 = f1 - f2;
                f0 = f1 * f0;
                *(f32*)((u8*)r28 + 0x14) = f0;

            } else {
                r3 = r28;
                r5 = (u32)sp + 0x8;
                fn_800FDF1C();
                r6 = r3;
                if ((u32)r6 == (u32)0x0) {
                    f0 = *(f32*)((u8*)r28 + 0x10);
                    r6 = (0x4330 << 16);
                    f1 = *(f32*)((u8*)r28 + 0xC);
                    r3 = (0x8000 << 16);
                    f0 = (f64)(s32)f0;
                    r0 = *(u8*)((u8*)r28 + 0x22);
                    f1 = (f64)(s32)f1;
                    r4 = *(u8*)((u8*)r28 + 0x23);
                    *(u32*)(sp + 0x24) = r0;
                    r3 = r3 + 0x2;
                    *(f64*)(sp + 0x18) = f0;
                    f6 = *(f64*)lbl_8047CD28;
                    *(f64*)(sp + 0x10) = f1;
                    r0 = r5 + 0x2;
                    r26 = (s16)r0;
                    r0 = (s16)r24;
                    /* xoris r5, r0, 0x8000 */;
                    /* xoris r0, r26, 0x8000 */;
                    f0 = *(f64*)(sp + 0x20);
                    f4 = *(f64*)lbl_8047CD10;
                    f5 = f0 - f6;
                    f0 = *(f64*)(sp + 0x28);
                    f2 = f0 - f4;
                    f3 = *(f32*)((u8*)r28 + 0x60);
                    f1 = *(f32*)((u8*)r28 + 0x64);
                    f0 = *(f64*)(sp + 0x38);
                    f3 = f5 * f3 + f2;
                    *(u32*)(sp + 0x44) = r0;
                    f2 = f0 - f6;
                    f3 = (f64)(s32)f3;
                    f0 = *(f64*)(sp + 0x40);
                    *(f64*)(sp + 0x30) = f3;
                    f0 = f0 - f4;
                    f0 = f2 * f1 + f0;
                    f0 = (f64)(s32)f0;
                    *(f64*)(sp + 0x48) = f0;
                    fn_800D888C();
                    r3 = 0x7;
                    fn_800D6A00();
                    r3 = (u32)lbl_80314E08;
                    r3 = (u32)lbl_80314E08;
                    fn_800D7820();
                    r3 = 0x2;
                    fn_800D67BC();
                    r3 = r24;
                    r4 = r26;
                    fn_800D61E4();
                    r3 = 0x0;
                    r4 = 0xff;
                    r5 = 0xff;
                    r6 = 0xff;
                    r7 = 0xff;
                    fn_800D5CB8();
                    r3 = r25;
                    r4 = r27;
                    fn_800D61E4();
                    r3 = 0x0;
                    r4 = 0xff;
                    r5 = 0xff;
                    r6 = 0xff;
                    r7 = 0xff;
                    fn_800D5CB8();
                    fn_800D6728();
                    r3 = (0x8000 << 16);
                    r3 = r3 + 0x2;
                    fn_800D88DC();
                    r3 = (u32)lbl_80314F98;
                    r3 = (u32)lbl_80314F98;
                    fn_800D7820();
                    r3 = 0x1;
                    fn_800DC1D4();
                    r3 = *(u8*)((u8*)r28 + 0x22);
                    r0 = (0x4330 << 16);
                    *(u32*)(sp + 0x50) = r0;
                    f2 = *(f64*)lbl_8047CD28;
                    f0 = *(f32*)((u8*)r28 + 0x60);
                    f1 = *(f64*)(sp + 0x50);
                    f3 = *(f32*)lbl_8047CD30;
                    f1 = f1 - f2;
                    f0 = f1 * f0;
                    f0 = f3 + f0;
                    *(f32*)((u8*)r28 + 0x14) = f0;

                } else {
                    r3 = r28;
                    r0 = *(u32*)((u8*)r6 + 0x4);
                    r5 = *(u32*)((u8*)r8 + 0x4);
                    r4 = r0 & 0xFFFFFF;
                    r27 = *(u8*)((u8*)r6 + 0x2);
                    r4 = r5 + r4;
                    r0 = (u32)r0 >> 24;
                    r6 = *(u8*)((u8*)r6 + 0x3);
                    r7 = (s8)r0;
                    r5 = r27;
                    r4 = r8 + r4;
                    fn_800FD69C();
                    r3 = (s16)r27;
                    r0 = (0x4330 << 16);
                    /* xoris r3, r3, 0x8000 */;
                    *(u32*)(sp + 0x50) = r0;
                    f2 = *(f64*)lbl_8047CD10;
                    f0 = *(f32*)((u8*)r28 + 0x60);
                    f1 = *(f64*)(sp + 0x50);
                    f1 = f1 - f2;
                    f0 = f1 * f0;
                    *(f32*)((u8*)r28 + 0x14) = f0;
                }
                }

            f1 = *(f32*)((u8*)r28 + 0xC);
            /* subi r30, r30, 0x1 */;
            f0 = *(f32*)((u8*)r28 + 0x14);
            f0 = f1 + f0;
            *(f32*)((u8*)r28 + 0xC) = f0;
        }
        L_800FD630: ;
        r0 = r30 & 0xFF;
    } while ((u32)r0 != (u32)0x0);
    f0 = *(f32*)((u8*)r28 + 0xC);
    f1 = *(f32*)lbl_8047CD30;
    *(f32*)((u8*)r28 + 0x4C) = f0;
    f0 = *(f32*)((u8*)r28 + 0x10);
    *(f32*)((u8*)r28 + 0x50) = f0;
    *(u32*)((u8*)r28 + 0x54) = r29;
    *(u8*)((u8*)r28 + 0x59) = r31;
    *(f32*)((u8*)r28 + 0xC) = f31;
    *(f32*)((u8*)r28 + 0x10) = f30;
    f0 = *(f32*)((u8*)r28 + 0x60);
    f0 = f0 * f1;
    *(f32*)((u8*)r28 + 0x60) = f0;
    f0 = *(f32*)((u8*)r28 + 0x64);
    f0 = f0 * f1;
    *(f32*)((u8*)r28 + 0x64) = f0;
    /* psq_l f31, 0x98((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x90);
    /* psq_l f30, 0x88((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x80);
    return;
}

/* 0x800FD69C | 0x880 */
void fn_800FD69C(void) {
    extern u8 lbl_8047CD10[];
    extern u8 lbl_8047CD4C[];
    extern void fn_800D59B8();
    extern void fn_800D5BA0();
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    u8 sp[0xE0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    *(f64*)(sp + 0xD0) = f31;
    /* psq_st f31, 0xd8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xC0) = f30;
    /* psq_st f30, 0xc8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xB0) = f29;
    /* psq_st f29, 0xb8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xA0) = f28;
    /* psq_st f28, 0xa8((u32)sp), 0, qr0 */;
    r31 = r3;
    r10 = *(u32*)&lbl_80478B08;
    r8 = (s16)r5;
    r0 = r8 + 0x2;
    r3 = *(s16*)((u8*)r10 + 0x18);
    r0 = r3 + r0;
    if ((s32)r0 >= (s32)0x200) {
        r9 = *(s16*)((u8*)r10 + 0x1A);
        r0 = 0x2;
        r3 = *(u8*)((u8*)r10 + 0x1C);
        r3 = r3 + r9;
        r3 = r3 + 0x2;
        r3 = (s16)r3;
        *(u16*)((u8*)r10 + 0x1A) = r3;
        r3 = *(u32*)&lbl_80478B08;
        *(u16*)((u8*)r3 + 0x18) = r0;
        r9 = *(u8*)((u8*)r31 + 0x23);
        r3 = *(u32*)&lbl_80478B08;
        r0 = r9 + 0x2;
        r0 = r0 & 0xFF;
        *(u8*)((u8*)r3 + 0x1C) = r0;
        r9 = *(u32*)&lbl_80478B08;
        r3 = *(s16*)((u8*)r9 + 0x1A);
        r0 = *(u8*)((u8*)r9 + 0x1C);
        r0 = r3 + r0;
        if ((s32)r0 >= (s32)0x200) {
            r0 = 0x1;
            *(u16*)((u8*)r9 + 0x1A) = r0;
    }
    }
    r3 = *(u32*)&lbl_80478B08;
    r9 = (s16)r6;
    r0 = *(u8*)((u8*)r3 + 0x1C);
    if ((s32)r0 < (s32)r9) {
        r0 = *(u8*)((u8*)r31 + 0x23);
        *(u8*)((u8*)r3 + 0x1C) = r0;
    }
    r0 = r8 + 0x1;
    r11 = *(u32*)&lbl_80478B08;
    /* clrrwi r10, r0, 1 */;
    r3 = r8 + 0x2;
    r0 = (u32)r10 >> 31;
    r12 = *(u32*)((u8*)r11 + 0x14);
    r10 = r0 + r10;
    r0 = r9 + 0x1;
    r30 = (s32)r10 >> 1;
    r11 = -0x1;
    while ((s32)r11 < (s32)r0) {
            r25 = *(u32*)&lbl_80478B08;
            r10 = -0x2;
            r25 = *(s16*)((u8*)r25 + 0x1A);
            r23 = r25 + r11;
            r26 = (s32)r23 >> 3;
            r25 = r23 & 0x7;
            r24 = r26 << 6;
            r29 = r25 << 3;
            r26 = r3 + 0x3;
            r26 = (u32)r26 >> 1;
            r28 = 0x0;
            if ((s32)r3 > (s32)-0x2) {
                r25 = (u32)r26 >> 2;
                ctr_fn = (void(*)(void))r25;
                if ((u32)r25 != (u32)0x0) {
                    do {
                        r25 = *(u32*)&lbl_80478B08;
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r23 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r23 & 0x7;
                        r27 = (s32)r23 >> 3;
                        r25 = r25 + r29;
                        r27 = r27 + r24;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                        r25 = *(u32*)&lbl_80478B08;
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r23 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r23 & 0x7;
                        r27 = (s32)r23 >> 3;
                        r25 = r25 + r29;
                        r27 = r27 + r24;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                        r25 = *(u32*)&lbl_80478B08;
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r23 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r23 & 0x7;
                        r27 = (s32)r23 >> 3;
                        r25 = r25 + r29;
                        r27 = r27 + r24;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                        r25 = *(u32*)&lbl_80478B08;
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r23 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r23 & 0x7;
                        r27 = (s32)r23 >> 3;
                        r25 = r25 + r29;
                        r27 = r27 + r24;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                    } while (--ctr != 0);
                    r26 = r26 & 0x3;
                    if ((u32)r25 != (u32)0x0) {
                    }
                    ctr_fn = (void(*)(void))r26;
                    do {
                        r25 = *(u32*)&lbl_80478B08;
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r23 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r23 & 0x7;
                        r27 = (s32)r23 >> 3;
                        r25 = r25 + r29;
                        r27 = r27 + r24;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                    } while (--ctr != 0);
                }
                    }
            r11 = r11 + 0x1;
    }
    r3 = 0x0;
    r0 = r3;
    while ((s32)r3 < (s32)r9) {
            r25 = *(u32*)&lbl_80478B08;
            r11 = r4 + r0;
            r10 = 0x0;
            r25 = *(s16*)((u8*)r25 + 0x1A);
            r23 = r25 + r3;
            r26 = (s32)r23 >> 3;
            r25 = r23 & 0x7;
            r23 = r26 << 6;
            r24 = r25 << 3;
            r26 = r8 + 0x1;
            r26 = (u32)r26 >> 1;
            if ((s32)r8 > (s32)0x0) {
                r25 = (u32)r26 >> 2;
                ctr_fn = (void(*)(void))r25;
                if ((u32)r25 != (u32)0x0) {
                    do {
                        r25 = *(u32*)&lbl_80478B08;
                        r28 = *(u8*)((u8*)r11 + 0x0);
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r27 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r27 & 0x7;
                        r27 = (s32)r27 >> 3;
                        r25 = r25 + r24;
                        r27 = r27 + r23;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                        r25 = *(u32*)&lbl_80478B08;
                        r28 = *(u8*)((u8*)r11 + 0x1);
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r27 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r27 & 0x7;
                        r27 = (s32)r27 >> 3;
                        r25 = r25 + r24;
                        r27 = r27 + r23;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                        r25 = *(u32*)&lbl_80478B08;
                        r28 = *(u8*)((u8*)r11 + 0x2);
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r27 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r27 & 0x7;
                        r27 = (s32)r27 >> 3;
                        r25 = r25 + r24;
                        r27 = r27 + r23;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                        r25 = *(u32*)&lbl_80478B08;
                        r28 = *(u8*)((u8*)r11 + 0x3);
                        r11 = r11 + 0x4;
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r27 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r27 & 0x7;
                        r27 = (s32)r27 >> 3;
                        r25 = r25 + r24;
                        r27 = r27 + r23;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                    } while (--ctr != 0);
                    r26 = r26 & 0x3;
                    if ((u32)r25 != (u32)0x0) {
                    }
                    ctr_fn = (void(*)(void))r26;
                    do {
                        r25 = *(u32*)&lbl_80478B08;
                        r28 = *(u8*)((u8*)r11 + 0x0);
                        r11 = r11 + 0x1;
                        r25 = *(s16*)((u8*)r25 + 0x18);
                        r27 = r25 + r10;
                        r10 = r10 + 0x2;
                        r25 = r27 & 0x7;
                        r27 = (s32)r27 >> 3;
                        r25 = r25 + r24;
                        r27 = r27 + r23;
                        r25 = (s32)r25 >> 1;
                        r27 = r27 << 5;
                        r25 = r25 + r12;
                        *(u8*)(r27 + r25) = r28;
                    } while (--ctr != 0);
                }
                    }
            r0 = r0 + r30;
            r3 = r3 + 0x1;
    }
    r3 = (s16)r7;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    r7 = *(u8*)((u8*)r31 + 0x43);
    r4 = (s16)r5;
    r3 = (s8)r7;
    r10 = *(u32*)&lbl_80478B08;
    *(u32*)(sp + 0x18) = r0;
    /* xoris r5, r3, 0x8000 */;
    f5 = *(f64*)lbl_8047CD10;
    r3 = (s16)r6;
    f0 = *(f64*)(sp + 0x18);
    /* xoris r6, r4, 0x8000 */;
    /* xoris r7, r3, 0x8000 */;
    f1 = f0 - f5;
    f6 = *(f32*)((u8*)r31 + 0x64);
    *(u32*)(sp + 0x10) = r0;
    f0 = *(f32*)((u8*)r31 + 0x10);
    f2 = *(f64*)(sp + 0x10);
    f0 = f1 * f6 + f0;
    r4 = *(s16*)((u8*)r10 + 0x18);
    f1 = f2 - f5;
    f2 = *(f32*)((u8*)r31 + 0xC);
    r10 = *(s16*)((u8*)r10 + 0x1A);
    r3 = r4 + r8;
    f0 = f1 + f0;
    /* xoris r5, r4, 0x8000 */;
    f1 = (f64)(s32)f2;
    /* xoris r4, r10, 0x8000 */;
    /* xoris r3, r3, 0x8000 */;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f1;
    f3 = *(f32*)((u8*)r31 + 0x60);
    *(f64*)(sp + 0x20) = f0;
    r6 = (s16)r28;
    f1 = *(f32*)lbl_8047CD4C;
    /* xoris r8, r6, 0x8000 */;
    *(u32*)(sp + 0x28) = r0;
    r6 = (s16)r26;
    /* xoris r6, r6, 0x8000 */;
    f0 = *(f64*)(sp + 0x28);
    f4 = f0 - f5;
    *(u32*)(sp + 0x30) = r0;
    f0 = *(f64*)(sp + 0x30);
    f2 = f0 - f5;
    *(u32*)(sp + 0x40) = r0;
    f0 = *(f64*)(sp + 0x40);
    f3 = f4 * f3 + f2;
    f2 = f0 - f5;
    *(u32*)(sp + 0x48) = r0;
    f3 = (f64)(s32)f3;
    f0 = *(f64*)(sp + 0x48);
    *(f64*)(sp + 0x38) = f3;
    f0 = f0 - f5;
    *(u32*)(sp + 0x58) = r0;
    f2 = f2 * f6 + f0;
    f0 = *(f64*)(sp + 0x58);
    f3 = (f64)(s32)f2;
    f2 = f0 - f5;
    *(u32*)(sp + 0x60) = r0;
    f0 = *(f64*)(sp + 0x60);
    f31 = f2 * f1;
    *(f64*)(sp + 0x50) = f3;
    f0 = f0 - f5;
    *(u32*)(sp + 0x68) = r0;
    f29 = f0 * f1;
    f0 = *(f64*)(sp + 0x68);
    f0 = f0 - f5;
    f30 = f0 * f1;
    r4 = *(u32*)((u8*)r31 + 0x24);
    r3 = r10 + r9;
    /* xoris r5, r3, 0x8000 */;
    r3 = (0x8081 << 16);
    r4 = r4 & 0xFF;
    r4 = r4 * 0xc0;
    /* subi r5, r3, 0x7f7f */;
    *(u32*)(sp + 0x70) = r0;
    r3 = 0x7;
    r0 = (u32)((u64)r5 * (u64)r4 >> 32);
    f0 = *(f64*)(sp + 0x70);
    f0 = f0 - f5;
    f28 = f0 * f1;
    r0 = (u32)r0 >> 7;
    r29 = r0 & 0xFF;
    fn_800D6A00();
    r0 = *(u8*)((u8*)r31 + 0x2);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 < (s32)0x1) {
        r3 = 0x2;
        fn_800D67BC();

    } else {
        r3 = 0x4;
        fn_800D67BC();
        r3 = r28 + 0x1;
        r0 = r26 + 0x1;
        r3 = (s16)r3;
        r4 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f31;
        r3 = 0x0;
        f2 = f29;
        fn_800D59B8();
        r3 = r27 + 0x1;
        r0 = r25 + 0x1;
        r3 = (s16)r3;
        r4 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f30;
        r3 = 0x0;
        f2 = f28;
        fn_800D59B8();
        goto L_800FDE78;
        r3 = 0xa;
        fn_800D67BC();
        r24 = (s16)r28;
        r4 = r26;
        /* subi r0, r24, 0x1 */;
        r3 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f31;
        r3 = 0x0;
        f2 = f29;
        fn_800D59B8();
        r23 = (s16)r27;
        r4 = r25;
        /* subi r0, r23, 0x1 */;
        r3 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f30;
        r3 = 0x0;
        f2 = f28;
        fn_800D59B8();
        r0 = r24 + 0x1;
        r4 = r26;
        r3 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f31;
        r3 = 0x0;
        f2 = f29;
        fn_800D59B8();
        r0 = r23 + 0x1;
        r4 = r25;
        r3 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f30;
        r3 = 0x0;
        f2 = f28;
        fn_800D59B8();
        r23 = (s16)r26;
        r3 = r28;
        /* subi r0, r23, 0x1 */;
        r4 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f31;
        r3 = 0x0;
        f2 = f29;
        fn_800D59B8();
        r24 = (s16)r25;
        r3 = r27;
        /* subi r0, r24, 0x1 */;
        r4 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f30;
        r3 = 0x0;
        f2 = f28;
        fn_800D59B8();
        r0 = r23 + 0x1;
        r3 = r28;
        r4 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f31;
        r3 = 0x0;
        f2 = f29;
        fn_800D59B8();
        r0 = r24 + 0x1;
        r3 = r27;
        r4 = (s16)r0;
        fn_800D61E4();
        r7 = r29;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        fn_800D5CB8();
        f1 = f30;
        r3 = 0x0;
        f2 = f28;
        fn_800D59B8();
    }
    L_800FDE78: ;
    r3 = r28;
    r4 = r26;
    fn_800D61E4();
    r4 = *(u32*)((u8*)r31 + 0x24);
    r3 = 0x0;
    fn_800D5BA0();
    f1 = f31;
    r3 = 0x0;
    f2 = f29;
    fn_800D59B8();
    r3 = r27;
    r4 = r25;
    fn_800D61E4();
    r4 = *(u32*)((u8*)r31 + 0x24);
    r3 = 0x0;
    fn_800D5BA0();
    f1 = f30;
    r3 = 0x0;
    f2 = f28;
    fn_800D59B8();
    fn_800D6728();
    r4 = *(u32*)&lbl_80478B08;
    r0 = r30 << 1;
    r3 = *(s16*)((u8*)r4 + 0x18);
    r3 = r0 + r3;
    r0 = r3 + 0x2;
    r0 = (s16)r0;
    *(u16*)((u8*)r4 + 0x18) = r0;
    /* psq_l f31, 0xd8((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0xD0);
    /* psq_l f30, 0xc8((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0xC0);
    /* psq_l f29, 0xb8((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0xB0);
    /* psq_l f28, 0xa8((u32)sp), 0, qr0 */;
    f28 = *(f64*)(sp + 0xA0);
    return;
}
}

/* 0x800FDF1C | 0xC8 */
void fn_800FDF1C(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r8 = *(u32*)&lbl_80478B08;
    r11 = 0x0;
    r7 = 0x0;
    r9 = *(u16*)((u8*)r8 + 0x4);
    ctr_fn = (void(*)(void))r9;
    if ((s32)r9 > (s32)0x0) {
        do {
            r6 = *(u32*)((u8*)r8 + 0x24);
            r0 = *(u16*)((u8*)r3 + 0x20);
            r10 = r6 + r7;
            r6 = *(u16*)((u8*)r10 + 0x0);
            if ((u32)r6 == (u32)r0) break;
            r7 = r7 + 0x8;
            r11 = r11 + 0x1;
        } while (--ctr != 0);
    }
    if ((s32)r11 == (s32)r9) {
        r3 = 0x0;
        return;
    }
    r6 = *(u32*)((u8*)r10 + 0x4);
    r4 = r4 & 0xFFFF;
    while ((u32)r6 != (u32)0x0) {
            r9 = *(u16*)((u8*)r6 + 0x0);
            r7 = r6 + 0x10;
            r8 = 0x0;
            goto L_800FDFC4;
            do {
                r0 = r8 + r9;
                r10 = (u32)r0 >> 1;
                r0 = r10 << 3;
                r3 = r7 + r0;
                r0 = *(u16*)((u8*)r3 + 0x0);
                if ((u32)r0 == (u32)r4) {
                    if ((u32)r5 == (u32)0x0) return;
                    *(u32*)((u8*)r5 + 0x0) = r6;
                    return;
                }
                if ((u32)r5 < (u32)0x0) {
                    r8 = r10 + 0x1;

                } else {
                    r9 = r10;
                }
                L_800FDFC4: ;
            } while ((u32)r8 < (u32)r9);
            if ((u32)r8 < (u32)r9) { r3 = 0x0; return; }
            r6 = *(u32*)((u8*)r6 + 0x8);
    }

    r3 = 0x0;
    return;
}

/* 0x800FDFE4 | 0x2C -- (fn_800FE010() + 1) >> 1 - 1 */
u32 fn_800FDFE4(void) {
    u32 result = (u32)fn_800FE010();
    return ((result + 1) >> 1) - 1;
}

/* 0x800FE010 | 0x34C */
u32 fn_800FE010(void) {
    extern u8 lbl_80271700[];
    extern u8 lbl_802717D4[];
    extern u8 lbl_80315678[];
    extern u8 lbl_80402480[];
    extern u8 lbl_8047CD08[];
    extern u8 lbl_8047CD18[];
    extern u8 lbl_8047CD20[];
    extern u8 lbl_8047CD28[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r29 = r3;
    if ((u32)r29 == (u32)0x0) {
        r3 = (u32)lbl_802717D4;
        r3 = (u32)lbl_802717D4;
        /* crclr cr1eq */;
        ((void(*)(void))fn_800DD970)();
        r3 = 0x0;
        return;
    }
    r3 = (u32)lbl_80402480;
    r4 = 0x0;
    r3 = (u32)lbl_80402480;
    r5 = 0x68;
    r30 = r3;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = (u32)lbl_80402480;
    f0 = *(f32*)lbl_8047CD08;
    r3 = (u32)lbl_80402480;
    r4 = 0x1;
    r0 = -0x1;
    *(u8*)((u8*)r3 + 0x0) = r4;
    r6 = *(u32*)&lbl_80478B08;
    r5 = 0x0;
    *(f32*)((u8*)r3 + 0x60) = f0;
    *(f32*)((u8*)r3 + 0x64) = f0;
    *(u32*)((u8*)r3 + 0x24) = r0;
    *(u32*)((u8*)r3 + 0x28) = r29;
    *(u32*)((u8*)r3 + 0x2C) = r29;
    *(u32*)((u8*)r3 + 0x30) = r29;
    *(u8*)((u8*)r3 + 0x1) = r4;
    r0 = *(u16*)((u8*)r6 + 0x4);
    r4 = *(u16*)((u8*)r30 + 0x20);
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 > (s32)0x0) {
        do {
            r0 = *(u32*)((u8*)r6 + 0x24);
            r3 = r0 + r5;
            r0 = *(u16*)((u8*)r3 + 0x0);
            if ((u32)r0 == (u32)r4) {
                r0 = *(u8*)((u8*)r3 + 0x2);
                *(u8*)((u8*)r30 + 0x22) = r0;
                r3 = *(u8*)((u8*)r3 + 0x3);
                *(u8*)((u8*)r30 + 0x23) = r3;
                if ((u32)r4 == (u32)0x0) {
                    r0 = 0xb;
                    *(u8*)((u8*)r30 + 0x42) = r0;
                    break;
                }

                if ((u32)r4 == (u32)0x1 && (u32)r4 != (u32)0x1) {

                    r0 = 0x6;
                    *(u8*)((u8*)r30 + 0x42) = r0;
                    break;
                }
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047CD28;
                *(u32*)(sp + 0x8) = r0;
                f3 = *(f64*)lbl_8047CD20;
                f1 = *(f64*)(sp + 0x8);
                f0 = *(f64*)lbl_8047CD18;
                f1 = f1 - f2;
                f0 = f3 * f1 + f0;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x10) = f0;
                r0 = (s8)r0;
                *(u8*)((u8*)r30 + 0x42) = r0;
                break;
            }
            r5 = r5 + 0x8;
        } while (--ctr != 0);
    }
    L_800FE14C: ;
    r3 = *(u32*)((u8*)r30 + 0x30);
    r4 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r4 == (u32)0x0) {
        r3 = *(u8*)((u8*)r30 + 0x40);
        r0 = (s8)r3;
        if ((s32)r0 == (s32)0x0) {
    r0 = r4 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) {
        if ((u32)r0 != (u32)0xffff) goto L_800FE14C;
        r4 = *(u32*)((u8*)r30 + 0x30);
        r3 = *(u32*)&lbl_80478B08;
        r0 = r4 + 0x1;
        *(u32*)((u8*)r30 + 0x30) = r0;
        r3 = *(u32*)((u8*)r3 + 0x28);
        r4 = *(u8*)((u8*)r4 + 0x0);
        if ((u32)r3 == (u32)0x0) goto L_800FE14C;
        r0 = *(u8*)((u8*)r30 + 0x1);
        if ((u32)r0 == (u32)0x0) {
            r0 = r4 << 3;
            r0 = *(u8*)(r3 + r0);
            /* extrwi r0, r0, 1, 27 */;
        } else {

            r0 = r4 << 3;
            r0 = *(u8*)(r3 + r0);
            /* extrwi r0, r0, 1, 28 */;
        }
        if ((u32)r0 == (u32)0x0) goto L_800FE14C;
        r0 = r4 << 3;
        r28 = r3 + r0;
        r12 = *(u32*)((u8*)r28 + 0x4);
        if ((u32)r12 == (u32)0x0) goto L_800FE14C;
        r3 = r30;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        r0 = *(u8*)((u8*)r28 + 0x0);
        /* extrwi r0, r0, 2, 24 */;
        if ((u32)r0 == (u32)0x0 || (u32)r3 == (u32)0x0) goto L_800FE14C;

        if ((s32)r0 != (s32)0x2) {
            if ((s32)r0 < (s32)0x2) {
                if ((s32)r0 < (s32)0x1) {

                } else {
                    r31 = r3;

                }
            } else {
                    if ((u32)r3 == (u32)0x0) {
                        r31 = 0x0;

                    } else {
                        r4 = *(u32*)&lbl_80478B08;
                        r5 = (u32)r3 >> 20;
                        r3 = r3 & 0xFFFFF;
                        r9 = *(u32*)((u8*)r4 + 0x8);
                        while ((u32)r9 != (u32)0x0) {
                                r0 = *(u16*)((u8*)r9 + 0x0);
                                if ((u32)r0 == (u32)r5) {
                                    r6 = *(u16*)((u8*)r9 + 0x4);
                                    r8 = r9 + 0x10;
                                    r7 = 0x0;
                                    goto L_800FE2CC;
                                    do {
                                        r0 = r7 + r6;
                                        r4 = (u32)r0 >> 1;
                                        r0 = r4 << 3;
                                        r10 = r8 + r0;
                                        r0 = *(u32*)((u8*)r10 + 0x0);
                                        if ((u32)r0 == (u32)r3) {
                                            r0 = *(u32*)((u8*)r10 + 0x4);
                                            r31 = r9 + r0;
                                            goto L_800FE2E4;
                                        }
                                        if ((u32)r0 < (u32)r3) {
                                            r7 = r4 + 0x1;

                                        } else {
                                            r6 = r4;
                                        }
                                        L_800FE2CC: ;
                                    } while ((u32)r7 < (u32)r6);
                                }
                                r9 = *(u32*)((u8*)r9 + 0x8);
                        }
                        r31 = 0x0;
                        }
                    }
                    }
        L_800FE2E4: ;
        r4 = *(u8*)((u8*)r30 + 0x40);
        r3 = (s8)r4;
        if ((s32)r3 >= (s32)0x3) {
            r3 = (u32)lbl_80271700;
            r4 = (u32)lbl_80315678;
            r3 = (u32)lbl_80271700;
            r4 = (u32)lbl_80315678;
            /* crclr cr1eq */;
            ((void(*)(void))fn_800DD970)();
            goto L_800FE14C;
        }
        r0 = r4 + 0x1;
        r3 = r3 << 2;
        *(u8*)((u8*)r30 + 0x40) = r0;
        r0 = r3 + 0x34;
        r3 = *(u32*)((u8*)r30 + 0x30);
        *(u32*)(r30 + r0) = r3;
        *(u32*)((u8*)r30 + 0x30) = r31;
        goto L_800FE14C;
    }
    r0 = *(u32*)((u8*)r30 + 0x30);
    r3 = r0 - r29;
    r3 = r3 + 0x2;

    return;
}
}
}

/* 0x800FE35C | 0x30 -- fn_800D9D68(0, 0, 0x27F, 0x1DF) -- set viewport 640x480 */
extern void fn_800D9D68(u32 x, u32 y, u32 w, u32 h);
void fn_800FE35C(void) {
    fn_800D9D68(0, 0, 0x27F, 0x1DF);
}

/* 0x800FE38C | 0x148 */
void fn_800FE38C(void) {
    extern u8 lbl_8047CD50[];
    extern u8 lbl_80478B10;
    extern u8 lbl_80478B14;
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;

    /* lha r0, lbl_8047AC70@sda21(r0) */;
    r7 = (0x4330 << 16);
    /* lha r8, lbl_8047AC72@sda21(r0) */;
    r3 = r0 + r3;
    /* xoris r0, r3, 0x8000 */;
    r4 = r8 + r4;
    *(u32*)(sp + 0xC) = r0;
    r5 = r3 + r5;
    r0 = r4 + r6;
    /* xoris r4, r4, 0x8000 */;
    /* xoris r3, r5, 0x8000 */;
    f3 = *(f64*)lbl_8047CD50;
    /* xoris r0, r0, 0x8000 */;
    f0 = *(f64*)(sp + 0x8);
    f1 = f0 - f3;
    f4 = *(f32*)&lbl_80478B10;
    f5 = *(f32*)&lbl_80478B14;
    f0 = *(f64*)(sp + 0x18);
    f2 = f1 * f4;
    f1 = f0 - f3;
    f2 = (f64)(s32)f2;
    f0 = *(f64*)(sp + 0x28);
    f1 = f1 * f5;
    *(f64*)(sp + 0x10) = f2;
    f0 = f0 - f3;
    f2 = (f64)(s32)f1;
    *(u32*)(sp + 0x3C) = r0;
    f1 = f0 * f4;
    f0 = *(f64*)(sp + 0x38);
    f1 = (f64)(s32)f1;
    *(f64*)(sp + 0x20) = f2;
    f0 = f0 - f3;
    *(f64*)(sp + 0x30) = f1;
    f0 = f0 * f5;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x40) = f0;
    if ((s32)r3 >= (s32)0x280) {
        r3 = 0x27f;
    }
    if ((s32)r4 >= (s32)0x1e0) {
        r4 = 0x1df;
    }
    if ((s32)r5 >= (s32)0x280) {
        r5 = 0x27f;
    }
    if ((s32)r0 >= (s32)0x1e0) {
        r0 = 0x1df;
    }
    if ((s32)r3 < (s32)0x0) {
        r3 = 0x0;
    }
    if ((s32)r4 < (s32)0x0) {
        r4 = 0x0;
    }
    if ((s32)r5 < (s32)0x0) {
        r5 = 0x0;
    }
    if ((s32)r0 < (s32)0x0) {
        r0 = 0x0;
    }
    r3 = r3 & 0xFFFF;
    r4 = r4 & 0xFFFF;
    r5 = r5 & 0xFFFF;
    r6 = r0 & 0xFFFF;
    ((void(*)(void))fn_800D9D68)();
    return;
}

/* 0x800FE4D4 | 0x1CC */
void fn_800FE4D4(void) {
    extern u8 lbl_8047CD50[];
    extern u8 lbl_8047CD58[];
    extern u8 lbl_8047CD5C[];
    extern u8 lbl_8047CD60[];
    extern u8 lbl_8047CD64[];
    extern u8 lbl_8047CD68[];
    extern u8 lbl_8047CD6C[];
    extern u8 lbl_8047CD70[];
    extern u8 lbl_8047CD74[];
    extern u8 lbl_8047CD78[];
    extern void fn_800CE220();
    extern void fn_800D7FE4();
    extern void fn_800D834C();
    extern void fn_800D888C();
    extern void fn_800D9BD0();
    extern void fn_800DA028();
    extern void fn_800DA100();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    extern void fn_800E01F4();
    extern void fn_800E0218();
    extern u8 lbl_80478B10;
    extern u8 lbl_80478B14;
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0xB0) = f31;
    /* psq_st f31, 0xb8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xA0) = f30;
    /* psq_st f30, 0xa8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x90) = f29;
    /* psq_st f29, 0x98((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x80) = f28;
    /* psq_st f28, 0x88((u32)sp), 0, qr0 */;
    f1 = *(f32*)lbl_8047CD58;
    f0 = *(f32*)&lbl_80478B10;
    f3 = *(f32*)lbl_8047CD5C;
    f2 = *(f32*)&lbl_80478B14;
    f29 = f1 / f0;
    f0 = *(f32*)lbl_8047CD60;
    f1 = *(f32*)lbl_8047CD64;
    f28 = f3 / f2;
    f31 = f29 * f0;
    f30 = f28 * f0;
    fn_800CE220();
    f0 = (f32)f1;
    r4 = (0x4330 << 16);
    /* lha r5, lbl_8047AC70@sda21(r0) */;
    r3 = (u32)sp + 0x20;
    /* lha r0, lbl_8047AC72@sda21(r0) */;
    f3 = f30 / f0;
    /* xoris r5, r5, 0x8000 */;
    /* xoris r0, r0, 0x8000 */;
    f2 = *(f64*)lbl_8047CD50;
    f0 = *(f64*)(sp + 0x60);
    *(u32*)(sp + 0x6C) = r0;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x68);
    f1 = f31 - f1;
    f0 = f0 - f2;
    f2 = f30 - f0;
    fn_800E01F4();
    /* lha r5, lbl_8047AC70@sda21(r0) */;
    r4 = (0x4330 << 16);
    /* lha r0, lbl_8047AC72@sda21(r0) */;
    r3 = (u32)sp + 0x14;
    /* xoris r5, r5, 0x8000 */;
    /* xoris r0, r0, 0x8000 */;
    f2 = *(f64*)lbl_8047CD50;
    f3 = *(f32*)lbl_8047CD68;
    f0 = *(f64*)(sp + 0x70);
    *(u32*)(sp + 0x7C) = r0;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x78);
    f1 = f31 - f1;
    f0 = f0 - f2;
    f2 = f30 - f0;
    fn_800E01F4();
    f1 = *(f32*)lbl_8047CD68;
    r3 = (u32)sp + 0x8;
    f2 = *(f32*)lbl_8047CD6C;
    f3 = f1;
    fn_800E01F4();
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x20;
    r5 = (u32)sp + 0x8;
    r6 = (u32)sp + 0x14;
    fn_800E0218();
    f0 = f29 / f28;
    f1 = *(f32*)lbl_8047CD70;
    f3 = *(f32*)lbl_8047CD74;
    f4 = *(f32*)lbl_8047CD78;
    f2 = -f0;
    fn_800D9BD0();
    fn_800D834C();
    r3 = (u32)sp + 0x2c;
    fn_800D7FE4();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = (0x8000 << 16);
    fn_800D888C();
    r3 = 0x2;
    r4 = 0x2;
    r5 = 0x1;
    fn_800DA2BC();
    r3 = 0x0;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x7;
    r8 = 0x0;
    fn_800DA100();
    r3 = 0x0;
    r4 = 0x2;
    r5 = 0x0;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    /* psq_l f31, 0xb8((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0xB0);
    /* psq_l f30, 0xa8((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0xA0);
    /* psq_l f29, 0x98((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0x90);
    /* psq_l f28, 0x88((u32)sp), 0, qr0 */;
    f28 = *(f64*)(sp + 0x80);
    return;
}

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
void fn_800FE714(void) {
    extern u8 lbl_8047AC7C[];
    extern u8 lbl_8047AC98[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    /* subi r0, r3, 0x1 */;
    r0 = r0 * 0x18;
    r3 = *(u32*)lbl_8047AC7C;
    r31 = r3 + r0;
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r4 != (u32)0x0) {
        r0 = *(u32*)((u8*)r31 + 0x4);
        *(u32*)((u8*)r4 + 0x4) = r0;
    }
    r4 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r4 != (u32)0x0) {
        r0 = *(u32*)((u8*)r31 + 0x0);
        *(u32*)((u8*)r4 + 0x0) = r0;
    }
    r0 = *(u32*)lbl_8047AC98;
    if ((u32)r0 == (u32)r31) {
        r0 = *(u32*)((u8*)r31 + 0x4);
        *(u32*)lbl_8047AC98 = r0;
    }
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x0) = r0;
    *(u32*)((u8*)r31 + 0x4) = r0;
    OSRestoreInterrupts();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x8) = r0;
    return;
}

/* 0x800FE7A0 | 0x94 */
void fn_800FE7A0(void) {
    extern u8 lbl_8047AC7C[];
    extern u8 lbl_8047AC94[];
    extern u8 lbl_8047AC98[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r3 = (0xaaab << 16);
    /* subi r31, r3, 0x5555 */;
    r4 = *(u32*)lbl_8047AC98;
    while ((u32)r4 != (u32)0x0) {

        r0 = *(u32*)((u8*)r4 + 0x8);
        r30 = *(u32*)((u8*)r4 + 0x4);
        if ((s32)r0 == (s32)0x1) {
            r0 = *(u8*)((u8*)r4 + 0xD);
            if ((u32)r0 == (u32)0x0) {
                r0 = *(u32*)lbl_8047AC7C;
                *(u32*)lbl_8047AC94 = r4;
                r0 = r4 - r0;
                r0 = (u32)((u64)r31 * (u64)r0 >> 32);
                r12 = *(u32*)((u8*)r4 + 0x14);
                r4 = *(u32*)((u8*)r4 + 0x10);
                r3 = (u32)r0 >> 4;
                r3 = r3 + 0x1;
                ctr_fn = (void(*)(void))r12;
                ctr_fn();
        }
        }
        r4 = r30;

    }
    r0 = 0x0;
    *(u32*)lbl_8047AC94 = r0;
    return;
}

/* 0x800FE834 | 0x17C */
void fn_800FE834(void) {
    extern u8 lbl_8047AC7C[];
    extern u8 lbl_8047AC80[];
    extern u8 lbl_8047AC84[];
    extern u8 lbl_8047AC98[];
    extern u8 lbl_8047AC9C[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((s32)r3 == (s32)0x2) {
        r0 = *(u32*)lbl_8047AC80;
        r7 = *(u32*)lbl_8047AC7C;
        r0 = r0 * 0x18;
        r8 = *(u32*)lbl_8047AC84;
        r31 = r7 + r0;
    } else {

        r31 = *(u32*)lbl_8047AC7C;
        r8 = *(u32*)lbl_8047AC80;
    }
    ctr_fn = (void(*)(void))r8;
    if ((u32)r8 != (u32)0x0) {
        /* do-while loop (assembly fall-through pattern) */
        {
            r0 = *(u32*)((u8*)r31 + 0x8);
            if ((s32)r0 == (s32)0x0) {
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x0) = r0;
    *(u32*)((u8*)r31 + 0x4) = r0;
    *(u32*)((u8*)r31 + 0x8) = r3;
    *(u8*)((u8*)r31 + 0xC) = r4;
    *(u8*)((u8*)r31 + 0xD) = r0;
    *(u32*)((u8*)r31 + 0x10) = r5;
    *(u32*)((u8*)r31 + 0x14) = r6;
    r0 = *(u32*)lbl_8047AC98;
    if ((u32)r0 == (u32)0x0) {
        *(u32*)lbl_8047AC98 = r31;

    } else {
        OSDisableInterrupts();
        r0 = *(u32*)((u8*)r31 + 0x8);
        if ((s32)r0 == (s32)0x2) {
            r0 = *(u32*)lbl_8047AC9C;
            *(u32*)((u8*)r31 + 0x4) = r0;
            *(u32*)lbl_8047AC9C = r31;

        } else {
            r5 = *(u32*)lbl_8047AC98;
                r6 = *(u32*)((u8*)r5 + 0x4);
                if ((u32)r6 != (u32)0x0) {
                    r4 = *(u8*)((u8*)r5 + 0xC);
                    r0 = *(u8*)((u8*)r31 + 0xC);
                }
            }
            if ((u32)r6 == (u32)0x0) {
                r4 = *(u8*)((u8*)r5 + 0xC);
                r0 = *(u8*)((u8*)r31 + 0xC);
                if ((u32)r4 < (u32)r0) {
                    *(u32*)((u8*)r31 + 0x0) = r5;
                    r0 = 0x0;
                    *(u32*)((u8*)r31 + 0x4) = r0;
                    *(u32*)((u8*)r5 + 0x4) = r31;

                } else {
                    }
                r4 = *(u32*)((u8*)r5 + 0x0);
                if ((u32)r4 != (u32)0x0) {
                    *(u32*)((u8*)r4 + 0x4) = r31;
                }
                r0 = *(u32*)((u8*)r5 + 0x0);
                *(u32*)((u8*)r31 + 0x0) = r0;
                *(u32*)((u8*)r31 + 0x4) = r5;
                *(u32*)((u8*)r5 + 0x0) = r31;
                r0 = *(u32*)lbl_8047AC98;
                if ((u32)r0 == (u32)r5) {
                    *(u32*)lbl_8047AC98 = r31;
                }
            }
                }

        OSRestoreInterrupts();
    }

    r0 = *(u32*)lbl_8047AC7C;
    r3 = (0xaaab << 16);
    /* subi r3, r3, 0x5555 */;
    r0 = r31 - r0;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r3 = (u32)r0 >> 4;
    r3 = r3 + 0x1;

    return;
}
}
}

/* 0x800FE9B0 | 0xC4 */
void fn_800FE9B0(void) {
    extern u8 lbl_8047AC78[];
    extern u8 lbl_8047AC7C[];
    extern u8 lbl_8047AC80[];
    extern u8 lbl_8047AC84[];
    extern u8 lbl_8047AC88[];
    extern u8 lbl_8047AC8C[];
    extern u8 lbl_8047AC90[];
    extern u8 lbl_8047AC94[];
    extern void fn_800E27B0();
    extern void fn_800E3534();
    extern void fn_800FEA74();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r5 = r3 + r4;
    r0 = 0x0;
    *(u32*)lbl_8047AC80 = r3;
    r3 = r5 * 0x18;
    *(u32*)lbl_8047AC84 = r4;
    *(u32*)lbl_8047AC88 = r5;
    *(u32*)lbl_8047AC94 = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AC78 = r3;
    if ((s32)r0 == (s32)0) return;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AC7C = r3;
    r4 = r5;
    r6 = 0x0;
    while (1) {
        r0 = *(u32*)lbl_8047AC88;
        if ((u32)r6 >= (u32)r0) break;
        r3 = *(u32*)lbl_8047AC7C;
        r0 = r5 + 0x8;
        r5 = r5 + 0x18;
        r6 = r6 + 0x1;
        *(u32*)(r3 + r0) = r4;

    }
    r3 = 0x2000;
    fn_800E3534();
    *(u16*)lbl_8047AC8C = r3;
    r3 = r3 & 0xFFFF;
    fn_800E27B0();
    r4 = r3;
    r3 = (u32)fn_800FEA74;
    *(u32*)lbl_8047AC90 = r4;
    r5 = r4 + 0x1ffc;
    r3 = (u32)fn_800FEA74;
    r4 = 0x0;
    r6 = 0x1ffc;
    ((void(*)(void))fn_800A263C)();
    r3 = (u32)fn_800FEBA0;
    r3 = (u32)fn_800FEBA0;
    ((void(*)(void))fn_800D30A0)();

    return;
}

/* 0x800FEA74 | 0x12C */
void fn_800FEA74(void) {
    extern u8 lbl_8047AC7C[];
    extern u8 lbl_8047AC94[];
    extern u8 lbl_8047AC98[];
    extern u8 lbl_8047AC9C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r3 = (0xaaab << 16);
    /* subi r31, r3, 0x5555 */;
    while (1) {
        r3 = *(u32*)lbl_8047AC98;
        while ((u32)r3 != (u32)0x0) {

            r0 = *(u32*)((u8*)r3 + 0x8);
            r30 = *(u32*)((u8*)r3 + 0x4);
            if ((s32)r0 == (s32)0x2) {
                r0 = *(u8*)((u8*)r3 + 0xD);
                if ((u32)r0 == (u32)0x0) {
                    r0 = *(u32*)lbl_8047AC7C;
                    *(u32*)lbl_8047AC94 = r3;
                    r0 = r3 - r0;
                    r0 = (u32)((u64)r31 * (u64)r0 >> 32);
                    r12 = *(u32*)((u8*)r3 + 0x14);
                    r4 = *(u32*)((u8*)r3 + 0x10);
                    r3 = (u32)r0 >> 4;
                    r3 = r3 + 0x1;
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
            }
            }
            r3 = r30;

        }
        r0 = 0x0;
        *(u32*)lbl_8047AC94 = r0;
        OSDisableInterrupts();
        r7 = *(u32*)lbl_8047AC9C;
        while ((u32)r7 != (u32)0x0) {
                r8 = *(u32*)((u8*)r7 + 0x4);
                r5 = *(u32*)lbl_8047AC98;
                    r6 = *(u32*)((u8*)r5 + 0x4);
                    if ((u32)r6 != (u32)0x0) {
                        r4 = *(u8*)((u8*)r5 + 0xC);
                        r0 = *(u8*)((u8*)r7 + 0xC);
                } while ((u32)r4 < (u32)r0);
                }
                if ((u32)r6 == (u32)0x0) {
                    r4 = *(u8*)((u8*)r5 + 0xC);
                    r0 = *(u8*)((u8*)r7 + 0xC);
                    if ((u32)r4 < (u32)r0) {
                        *(u32*)((u8*)r7 + 0x0) = r5;
                        r0 = 0x0;
                        *(u32*)((u8*)r7 + 0x4) = r0;
                        *(u32*)((u8*)r5 + 0x4) = r7;

                    } else {
                        }
                    r4 = *(u32*)((u8*)r5 + 0x0);
                    if ((u32)r4 != (u32)0x0) {
                        *(u32*)((u8*)r4 + 0x4) = r7;
                    }
                    r0 = *(u32*)((u8*)r5 + 0x0);
                    *(u32*)((u8*)r7 + 0x0) = r0;
                    *(u32*)((u8*)r7 + 0x4) = r5;
                    *(u32*)((u8*)r5 + 0x0) = r7;
                    r0 = *(u32*)lbl_8047AC98;
                    if ((u32)r0 == (u32)r5) {
                        *(u32*)lbl_8047AC98 = r7;
                    }
                    }

                r7 = r8;
        }
        r0 = 0x0;
        *(u32*)lbl_8047AC9C = r0;
        OSRestoreInterrupts();
    }
