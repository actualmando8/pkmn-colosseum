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
extern void fn_800AB150(void);
extern void fn_800D0F44(void);
extern void fn_800AB4FC(void);
extern void fn_800E209C(void);
extern void fn_800E24B0(void);
extern void fn_800E27B0(void);
extern void fn_800E2C04(void);
extern void fn_800E3534(void);
extern void fn_80080ED8(void);
extern void fn_800DBEB4(void);
extern void fn_800D5CB8(void);
extern void fn_800D61E4(void);
extern void fn_800D6728(void);
extern void fn_800D67BC(void);
extern void fn_800D6A00(void);
extern void fn_800D7820(void);
extern void fn_800D85D4(void);
extern void fn_800D888C(void);
extern void fn_800D88DC(void);
extern void fn_800D9ED8(void);
extern void fn_800DC1D4(void);
extern void fn_800DE680(void);
extern void fn_800EF504(void);
extern void fn_800EF548(void);
extern void fn_801669BC(void);
extern void fn_800EF5FC(void);
extern void fn_800CDBE0(void);
extern void fn_800D3088(void);
extern void fn_800DBF78(void);
extern void fn_800DBFD4(void);
extern void fn_800DC04C(void);
extern void fn_800DC0D4(void);
extern void fn_800DC14C(void);
extern void fn_800DC224(void);
extern void fn_801040F0(void);
extern void fn_80166A28(void);
extern void fn_800D59B8(void);
extern void fn_800D5BA0(void);
extern void fn_800D9D68(void);
extern void fn_800CE220(void);
extern void fn_800D7FE4(void);
extern void fn_800D834C(void);
extern void fn_800D9BD0(void);
extern void fn_800DA028(void);
extern void fn_800DA100(void);
extern void fn_800DA1E8(void);
extern void fn_800DA2BC(void);
extern void fn_800DA4C4(void);
extern void fn_800E01F4(void);
extern void fn_800E0218(void);
extern void* memset(void* dest, int val, u32 n);
extern void* memcpy(void* dst, const void* src, u32 n);

/* ===== BSS/SDA symbol externs (for asm{} blocks) ===== */
/* BSS/data/rodata symbols accessed via lis/@ha + addi/@l pairs */
extern u32 lbl_80401C10;
/* .bss symbols */
extern u8  lbl_80401DE0[];
extern u8  lbl_80401E48[];
extern u8  lbl_80402418[];
extern u8  lbl_80402480[];
extern u8  lbl_804024E8[];
/* .data symbols */
extern u8  lbl_80314E08[];
extern u8  lbl_80314F98[];
extern u8  lbl_80315678[];
/* .rodata symbols */
extern u8  lbl_80271300[];
extern u8  lbl_80271500[];
extern u8  lbl_80271700[];
extern u8  lbl_80271730[];
extern u8  lbl_80271754[];
extern u8  lbl_8027177C[];
extern u8  lbl_802717B4[];
extern u8  lbl_802717D4[];
/* .sdata symbol */
extern float lbl_80478AC0;
/* sdata2 (r2) float/double constants used in asm blocks */
extern f64 lbl_8047CCC8;  /* f64 */
extern f32 lbl_8047CCD0;  /* f32 */
extern f32 lbl_8047CCD4;  /* f32 */
extern f32 lbl_8047CCD8;  /* f32 */
extern f32 lbl_8047CCDC;  /* f32 */
extern f64 lbl_8047CCE0;  /* f64 */
extern f64 lbl_8047CCE8;  /* f64 */
extern f64 lbl_8047CCF0;  /* f64 */
extern f64 lbl_8047CCF8;  /* f64 */
extern u32 lbl_8047CD00;  /* u32 (lwz) */
extern u32 lbl_8047CD04;  /* u32 (lwz) */
extern f32 lbl_8047CD08;  /* f32 */
extern f64 lbl_8047CD10;  /* f64 */
extern f64 lbl_8047CD18;  /* f64 */
extern f64 lbl_8047CD20;  /* f64 */
extern f64 lbl_8047CD28;  /* f64 */
extern f32 lbl_8047CD30;  /* f32 */
extern f32 lbl_8047CD34;  /* f32 */
extern f32 lbl_8047CD38;  /* f32 */
extern f32 lbl_8047CD3C;  /* f32 */
extern f32 lbl_8047CD40;  /* f32 */
extern f32 lbl_8047CD44;  /* f32 */
extern f32 lbl_8047CD48;  /* f32 */
extern f32 lbl_8047CD4C;  /* f32 */
extern f64 lbl_8047CD50;  /* f64 */
extern f32 lbl_8047CD58;  /* f32 */
extern f32 lbl_8047CD5C;  /* f32 */
extern f32 lbl_8047CD60;  /* f32 */
extern f32 lbl_8047CD64;  /* f32 */
extern f32 lbl_8047CD68;  /* f32 */
extern f32 lbl_8047CD6C;  /* f32 */
extern f32 lbl_8047CD70;  /* f32 */
extern f32 lbl_8047CD74;  /* f32 */
extern f32 lbl_8047CD78;  /* f32 */
/* sbss (r13) symbols -- task and thread system */
extern u32 lbl_80478B08;
extern u32 lbl_80478B10;
extern u32 lbl_80478B14;
extern u32 lbl_8047AC00;
extern u32 lbl_8047AC04;
extern u32 lbl_8047AC08;
extern u32 lbl_8047AC0C;
extern u32 lbl_8047AC10;
extern u32 lbl_8047AC14;
extern u32 lbl_8047AC18;
extern u32 lbl_8047AC1C;
extern u32 lbl_8047AC20;
extern u32 lbl_8047AC24;
extern u32 lbl_8047AC28;
extern u32 lbl_8047AC2C;
extern u32 lbl_8047AC30;
extern u32 lbl_8047AC34;
extern u32 lbl_8047AC38;
extern u32 lbl_8047AC3C;
extern u32 lbl_8047AC40;
extern u32 lbl_8047AC44;
extern u32 lbl_8047AC48;
extern u32 lbl_8047AC4C;
extern u32 lbl_8047AC50;
extern u32 lbl_8047AC54;
extern u32 lbl_8047AC58;
extern u32 lbl_8047AC5C;
extern u32 lbl_8047AC60;
extern u32 lbl_8047AC64;
extern u32 lbl_8047AC68;
extern u32 lbl_8047AC6C;
extern u32 lbl_8047AC70;
extern u32 lbl_8047AC72;
extern u32 lbl_8047AC74;
extern u32 lbl_8047AC78;
extern u32 lbl_8047AC7C;
extern u32 lbl_8047AC80;
extern u32 lbl_8047AC84;
extern u32 lbl_8047AC88;
extern u32 lbl_8047AC8C;
extern u32 lbl_8047AC90;
extern u32 lbl_8047AC94;
extern u32 lbl_8047AC98;
extern u32 lbl_8047AC9C;
/* sdata2 (r2) symbols: CW asm{} does not support sym(r2) syntax.
 * These are handled via numeric offsets in the .inc files.
 * No extern declarations needed. */

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
 * Generated: 1 pattern-matched + 61 stubs
 * Range: 0x800F8268 - 0x800FEBA0
 * =================================================================== */

/* Forward declarations for all asm-wrapped functions in this block */
extern void fn_800F8268(void);
extern void fn_800F8428(void);
extern void fn_800F8654(void);
extern void fn_800F8A54(void);
extern void fn_800F915C(void);
extern void fn_800F9210(void);
extern void fn_800F92D4(void);
extern void fn_800F9318(void);
extern void fn_800F9378(void);
extern void fn_800F9418(void);
extern void fn_800F9544(void);
extern void fn_800F9670(void);
extern void fn_800F96E4(void);
extern void fn_800F9AEC(void);
extern void fn_800F9C04(void);
extern void fn_800F9D04(void);
extern void fn_800F9D24(void);
extern void fn_800F9E70(void);
extern void fn_800F9EE4(void);
extern void fn_800FA064(void);
extern void fn_800FA160(void);
extern void fn_800FA1BC(void);
extern void fn_800FA280(void);
extern void fn_800FA314(void);
extern void fn_800FA3D0(void);
extern void fn_800FA444(void);
extern void fn_800FAA98(void);
extern void fn_800FAEF8(void);
extern void fn_800FB43C(void);
extern void fn_800FB680(void);
extern void fn_800FB8C8(void);
extern void fn_800FBB34(void);
extern void fn_800FBD88(void);
extern void fn_800FBE7C(void);
extern void fn_800FBF10(void);
extern void fn_800FBF74(void);
extern void fn_800FC1D0(void);
extern void fn_800FC244(void);
extern void fn_800FC2A4(void);
extern void fn_800FC2A8(void);
extern void fn_800FC39C(void);
extern void fn_800FC518(void);
extern void fn_800FC528(void);
extern void fn_800FC7E0(void);
extern void fn_800FD348(void);
extern void fn_800FD69C(void);
extern void fn_800FDF1C(void);
extern void fn_800FDFE4(void);
extern void fn_800FE010(void);
extern void fn_800FE35C(void);
extern void fn_800FE38C(void);
extern void fn_800FE4D4(void);
extern void fn_800FE6A0(void);
extern void fn_800FE6AC(void);
extern void fn_800FE6D0(void);
extern void fn_800FE6DC(void);
extern void fn_800FE6F8(void);
extern void fn_800FE714(void);
extern void fn_800FE7A0(void);
extern void fn_800FE834(void);
extern void fn_800FE9B0(void);
extern void fn_800FEA74(void);

/* 0x800F8268 | 0x1C0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F8268(void) {
#include "src/game/gs_thread_fn_800F8268.inc"
}
#else
void fn_800F8268(void) {
    /* TODO: match -- 448 bytes at 0x800F8268 */
}
#endif
#pragma pop

/* 0x800F8428 | 0x22C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F8428(void) {
#include "src/game/gs_thread_fn_800F8428.inc"
}
#else
void fn_800F8428(void) {
    /* TODO: match -- 556 bytes at 0x800F8428 */
}
#endif
#pragma pop

/* 0x800F8654 | 0x400 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F8654(void) {
#include "src/game/gs_thread_fn_800F8654.inc"
}
#else
void fn_800F8654(void) {
    /* TODO: match -- 1024 bytes at 0x800F8654 */
}
#endif
#pragma pop

/* 0x800F8A54 | 0x708 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F8A54(void) {
#include "src/game/gs_thread_fn_800F8A54.inc"
}
#else
void fn_800F8A54(void) {
    /* TODO: match -- 1800 bytes at 0x800F8A54 */
}
#endif
#pragma pop

/* 0x800F915C | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F915C(void) {
#include "src/game/gs_thread_fn_800F915C.inc"
}
#else
void fn_800F915C(void) {
    /* TODO: match -- 180 bytes at 0x800F915C */
}
#endif
#pragma pop

/* 0x800F9210 | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9210(void) {
#include "src/game/gs_thread_fn_800F9210.inc"
}
#else
void fn_800F9210(void) {
    /* TODO: match -- 196 bytes at 0x800F9210 */
}
#endif
#pragma pop

/* 0x800F92D4 | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F92D4(void) {
#include "src/game/gs_thread_fn_800F92D4.inc"
}
#else
void fn_800F92D4(void) {
    /* TODO: match -- 68 bytes at 0x800F92D4 */
}
#endif
#pragma pop

/* 0x800F9318 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9318(void) {
#include "src/game/gs_thread_fn_800F9318.inc"
}
#else
void fn_800F9318(void) {
    /* TODO: match -- 96 bytes at 0x800F9318 */
}
#endif
#pragma pop

/* 0x800F9378 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9378(void) {
#include "src/game/gs_thread_fn_800F9378.inc"
}
#else
void fn_800F9378(void) {
    /* TODO: match -- 160 bytes at 0x800F9378 */
}
#endif
#pragma pop

/* 0x800F9418 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9418(void) {
#include "src/game/gs_thread_fn_800F9418.inc"
}
#else
void fn_800F9418(void) {
    /* TODO: match -- 300 bytes at 0x800F9418 */
}
#endif
#pragma pop

/* 0x800F9544 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9544(void) {
#include "src/game/gs_thread_fn_800F9544.inc"
}
#else
void fn_800F9544(void) {
    /* TODO: match -- 300 bytes at 0x800F9544 */
}
#endif
#pragma pop

/* 0x800F9670 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9670(void) {
#include "src/game/gs_thread_fn_800F9670.inc"
}
#else
void fn_800F9670(void) {
    /* TODO: match -- 116 bytes at 0x800F9670 */
}
#endif
#pragma pop

/* 0x800F96E4 | 0x408 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F96E4(void) {
#include "src/game/gs_thread_fn_800F96E4.inc"
}
#else
void fn_800F96E4(void) {
    /* TODO: match -- 1032 bytes at 0x800F96E4 */
}
#endif
#pragma pop

/* 0x800F9AEC | 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9AEC(void) {
#include "src/game/gs_thread_fn_800F9AEC.inc"
}
#else
void fn_800F9AEC(void) {
    /* TODO: match -- 280 bytes at 0x800F9AEC */
}
#endif
#pragma pop

/* 0x800F9C04 | 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9C04(void) {
#include "src/game/gs_thread_fn_800F9C04.inc"
}
#else
void fn_800F9C04(void) {
    /* TODO: match -- 256 bytes at 0x800F9C04 */
}
#endif
#pragma pop

/* 0x800F9D04 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9D04(void) {
#include "src/game/gs_thread_fn_800F9D04.inc"
}
#else
void fn_800F9D04(void) {
    /* TODO: match -- 32 bytes at 0x800F9D04 */
}
#endif
#pragma pop

/* 0x800F9D24 | 0x14C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9D24(void) {
#include "src/game/gs_thread_fn_800F9D24.inc"
}
#else
void fn_800F9D24(void) {
    /* TODO: match -- 332 bytes at 0x800F9D24 */
}
#endif
#pragma pop

/* 0x800F9E70 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9E70(void) {
#include "src/game/gs_thread_fn_800F9E70.inc"
}
#else
void fn_800F9E70(void) {
    /* TODO: match -- 116 bytes at 0x800F9E70 */
}
#endif
#pragma pop

/* 0x800F9EE4 | 0x180 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800F9EE4(void) {
#include "src/game/gs_thread_fn_800F9EE4.inc"
}
#else
void fn_800F9EE4(void) {
    /* TODO: match -- 384 bytes at 0x800F9EE4 */
}
#endif
#pragma pop

/* 0x800FA064 | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FA064(void) {
#include "src/game/gs_thread_fn_800FA064.inc"
}
#else
void fn_800FA064(void) {
    /* TODO: match -- 252 bytes at 0x800FA064 */
}
#endif
#pragma pop

/* 0x800FA160 | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FA160(void) {
#include "src/game/gs_thread_fn_800FA160.inc"
}
#else
void fn_800FA160(void) {
    /* TODO: match -- 92 bytes at 0x800FA160 */
}
#endif
#pragma pop

/* 0x800FA1BC | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FA1BC(void) {
#include "src/game/gs_thread_fn_800FA1BC.inc"
}
#else
void fn_800FA1BC(void) {
    /* TODO: match -- 196 bytes at 0x800FA1BC */
}
#endif
#pragma pop

/* 0x800FA280 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FA280(void) {
#include "src/game/gs_thread_fn_800FA280.inc"
}
#else
void fn_800FA280(void) {
    /* TODO: match -- 148 bytes at 0x800FA280 */
}
#endif
#pragma pop

/* 0x800FA314 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FA314(void) {
#include "src/game/gs_thread_fn_800FA314.inc"
}
#else
void fn_800FA314(void) {
    /* TODO: match -- 188 bytes at 0x800FA314 */
}
#endif
#pragma pop

/* 0x800FA3D0 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FA3D0(void) {
#include "src/game/gs_thread_fn_800FA3D0.inc"
}
#else
void fn_800FA3D0(void) {
    /* TODO: match -- 116 bytes at 0x800FA3D0 */
}
#endif
#pragma pop

/* 0x800FA444 | 0x654 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FA444(void) {
#include "src/game/gs_thread_fn_800FA444.inc"
}
#else
void fn_800FA444(void) {
    /* TODO: match -- 1620 bytes at 0x800FA444 */
}
#endif
#pragma pop

/* 0x800FAA98 | 0x460 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FAA98(void) {
#include "src/game/gs_thread_fn_800FAA98.inc"
}
#else
void fn_800FAA98(void) {
    /* TODO: match -- 1120 bytes at 0x800FAA98 */
}
#endif
#pragma pop

/* 0x800FAEF8 | 0x544 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FAEF8(void) {
#include "src/game/gs_thread_fn_800FAEF8.inc"
}
#else
void fn_800FAEF8(void) {
    /* TODO: match -- 1348 bytes at 0x800FAEF8 */
}
#endif
#pragma pop

/* 0x800FB43C | 0x244 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FB43C(void) {
#include "src/game/gs_thread_fn_800FB43C.inc"
}
#else
void fn_800FB43C(void) {
    /* TODO: match -- 580 bytes at 0x800FB43C */
}
#endif
#pragma pop

/* 0x800FB680 | 0x248 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FB680(void) {
#include "src/game/gs_thread_fn_800FB680.inc"
}
#else
void fn_800FB680(void) {
    /* TODO: match -- 584 bytes at 0x800FB680 */
}
#endif
#pragma pop

/* 0x800FB8C8 | 0x26C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FB8C8(void) {
#include "src/game/gs_thread_fn_800FB8C8.inc"
}
#else
void fn_800FB8C8(void) {
    /* TODO: match -- 620 bytes at 0x800FB8C8 */
}
#endif
#pragma pop

/* 0x800FBB34 | 0x254 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FBB34(void) {
#include "src/game/gs_thread_fn_800FBB34.inc"
}
#else
void fn_800FBB34(void) {
    /* TODO: match -- 596 bytes at 0x800FBB34 */
}
#endif
#pragma pop

/* 0x800FBD88 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FBD88(void) {
#include "src/game/gs_thread_fn_800FBD88.inc"
}
#else
void fn_800FBD88(void) {
    /* TODO: match -- 244 bytes at 0x800FBD88 */
}
#endif
#pragma pop

/* 0x800FBE7C | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FBE7C(void) {
#include "src/game/gs_thread_fn_800FBE7C.inc"
}
#else
void fn_800FBE7C(void) {
    /* TODO: match -- 148 bytes at 0x800FBE7C */
}
#endif
#pragma pop

/* 0x800FBF10 | 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FBF10(void) {
#include "src/game/gs_thread_fn_800FBF10.inc"
}
#else
void fn_800FBF10(void) {
    /* TODO: match -- 100 bytes at 0x800FBF10 */
}
#endif
#pragma pop

/* 0x800FBF74 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FBF74(void) {
#include "src/game/gs_thread_fn_800FBF74.inc"
}
#else
void fn_800FBF74(void) {
    /* TODO: match -- 604 bytes at 0x800FBF74 */
}
#endif
#pragma pop

/* 0x800FC1D0 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FC1D0(void) {
#include "src/game/gs_thread_fn_800FC1D0.inc"
}
#else
void fn_800FC1D0(void) {
    /* TODO: match -- 116 bytes at 0x800FC1D0 */
}
#endif
#pragma pop

/* 0x800FC244 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FC244(void) {
#include "src/game/gs_thread_fn_800FC244.inc"
}
#else
void fn_800FC244(void) {
    /* TODO: match -- 96 bytes at 0x800FC244 */
}
#endif
#pragma pop

/* 0x800FC2A4 | 0x4 | void_stub */
#if 1
asm void fn_800FC2A4(void) {
#include "src/game/gs_thread_fn_800FC2A4.inc"
}
#else
void fn_800FC2A4(void) {
}
#endif

/* 0x800FC2A8 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FC2A8(void) {
#include "src/game/gs_thread_fn_800FC2A8.inc"
}
#else
void fn_800FC2A8(void) {
    /* TODO: match -- 244 bytes at 0x800FC2A8 */
}
#endif
#pragma pop

/* 0x800FC39C | 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FC39C(void) {
#include "src/game/gs_thread_fn_800FC39C.inc"
}
#else
void fn_800FC39C(void) {
    /* TODO: match -- 380 bytes at 0x800FC39C */
}
#endif
#pragma pop

/* 0x800FC518 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FC518(void) {
#include "src/game/gs_thread_fn_800FC518.inc"
}
#else
void fn_800FC518(void) {
    /* TODO: match -- 16 bytes at 0x800FC518 */
}
#endif
#pragma pop

/* 0x800FC528 | 0x2B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FC528(void) {
#include "src/game/gs_thread_fn_800FC528.inc"
}
#else
void fn_800FC528(void) {
    /* TODO: match -- 696 bytes at 0x800FC528 */
}
#endif
#pragma pop

/* 0x800FC7E0 | 0xB68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FC7E0(void) {
#include "src/game/gs_thread_fn_800FC7E0.inc"
}
#else
void fn_800FC7E0(void) {
    /* TODO: match -- 2920 bytes at 0x800FC7E0 */
}
#endif
#pragma pop

/* 0x800FD348 | 0x354 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FD348(void) {
#include "src/game/gs_thread_fn_800FD348.inc"
}
#else
void fn_800FD348(void) {
    /* TODO: match -- 852 bytes at 0x800FD348 */
}
#endif
#pragma pop

/* 0x800FD69C | 0x880 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FD69C(void) {
#include "src/game/gs_thread_fn_800FD69C.inc"
}
#else
void fn_800FD69C(void) {
    /* TODO: match -- 2176 bytes at 0x800FD69C */
}
#endif
#pragma pop

/* 0x800FDF1C | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FDF1C(void) {
#include "src/game/gs_thread_fn_800FDF1C.inc"
}
#else
void fn_800FDF1C(void) {
    /* TODO: match -- 200 bytes at 0x800FDF1C */
}
#endif
#pragma pop

/* 0x800FDFE4 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FDFE4(void) {
#include "src/game/gs_thread_fn_800FDFE4.inc"
}
#else
void fn_800FDFE4(void) {
    /* TODO: match -- 44 bytes at 0x800FDFE4 */
}
#endif
#pragma pop

/* 0x800FE010 | 0x34C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE010(void) {
#include "src/game/gs_thread_fn_800FE010.inc"
}
#else
void fn_800FE010(void) {
    /* TODO: match -- 844 bytes at 0x800FE010 */
}
#endif
#pragma pop

/* 0x800FE35C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE35C(void) {
#include "src/game/gs_thread_fn_800FE35C.inc"
}
#else
void fn_800FE35C(void) {
    /* TODO: match -- 48 bytes at 0x800FE35C */
}
#endif
#pragma pop

/* 0x800FE38C | 0x148 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE38C(void) {
#include "src/game/gs_thread_fn_800FE38C.inc"
}
#else
void fn_800FE38C(void) {
    /* TODO: match -- 328 bytes at 0x800FE38C */
}
#endif
#pragma pop

/* 0x800FE4D4 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE4D4(void) {
#include "src/game/gs_thread_fn_800FE4D4.inc"
}
#else
void fn_800FE4D4(void) {
    /* TODO: match -- 460 bytes at 0x800FE4D4 */
}
#endif
#pragma pop

/* 0x800FE6A0 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE6A0(void) {
#include "src/game/gs_thread_fn_800FE6A0.inc"
}
#else
void fn_800FE6A0(void) {
    /* TODO: match -- 12 bytes at 0x800FE6A0 */
}
#endif
#pragma pop

/* 0x800FE6AC | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE6AC(void) {
#include "src/game/gs_thread_fn_800FE6AC.inc"
}
#else
void fn_800FE6AC(void) {
    /* TODO: match -- 36 bytes at 0x800FE6AC */
}
#endif
#pragma pop

/* 0x800FE6D0 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE6D0(void) {
#include "src/game/gs_thread_fn_800FE6D0.inc"
}
#else
void fn_800FE6D0(void) {
    /* TODO: match -- 12 bytes at 0x800FE6D0 */
}
#endif
#pragma pop

/* 0x800FE6DC | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE6DC(void) {
#include "src/game/gs_thread_fn_800FE6DC.inc"
}
#else
void fn_800FE6DC(void) {
    /* TODO: match -- 28 bytes at 0x800FE6DC */
}
#endif
#pragma pop

/* 0x800FE6F8 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE6F8(void) {
#include "src/game/gs_thread_fn_800FE6F8.inc"
}
#else
void fn_800FE6F8(void) {
    /* TODO: match -- 28 bytes at 0x800FE6F8 */
}
#endif
#pragma pop

/* 0x800FE714 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE714(void) {
#include "src/game/gs_thread_fn_800FE714.inc"
}
#else
void fn_800FE714(void) {
    /* TODO: match -- 140 bytes at 0x800FE714 */
}
#endif
#pragma pop

/* 0x800FE7A0 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE7A0(void) {
#include "src/game/gs_thread_fn_800FE7A0.inc"
}
#else
void fn_800FE7A0(void) {
    /* TODO: match -- 148 bytes at 0x800FE7A0 */
}
#endif
#pragma pop

/* 0x800FE834 | 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE834(void) {
#include "src/game/gs_thread_fn_800FE834.inc"
}
#else
void fn_800FE834(void) {
    /* TODO: match -- 380 bytes at 0x800FE834 */
}
#endif
#pragma pop

/* 0x800FE9B0 | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FE9B0(void) {
#include "src/game/gs_thread_fn_800FE9B0.inc"
}
#else
void fn_800FE9B0(void) {
    /* TODO: match -- 196 bytes at 0x800FE9B0 */
}
#endif
#pragma pop

/* 0x800FEA74 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_800FEA74(void) {
#include "src/game/gs_thread_fn_800FEA74.inc"
}
#else
void fn_800FEA74(void) {
    /* TODO: match -- 300 bytes at 0x800FEA74 */
}
#endif
#pragma pop
