#include "dolphin/os/OSThread.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSTime.h"

/*
 * OSThread.c - Thread management.
 *
 * Adapted from doldecomp/melee matching implementation.
 * Implements thread initialization, scheduling, and thread queue management.
 *
 * Matches: 0x800A1290 - 0x800A2774
 */

#define ENQUEUE_THREAD(thread, queue, link)       \
    do {                                          \
        OSThread* __prev = (queue)->tail;         \
        if (__prev == NULL) {                     \
            (queue)->head = (thread);             \
        } else {                                  \
            __prev->link.next = (thread);         \
        }                                         \
        (thread)->link.prev = __prev;             \
        (thread)->link.next = 0;                  \
        (queue)->tail = (thread);                 \
    } while(0);

#define DEQUEUE_THREAD(thread, queue, link)             \
    do {                                                \
        OSThread* __next = (thread)->link.next;         \
        OSThread* __prev = (thread)->link.prev;         \
        if (__next == NULL) {                           \
            (queue)->tail = __prev;                     \
        } else {                                        \
            __next->link.prev = __prev;                 \
        }                                               \
        if (__prev == NULL) {                           \
            (queue)->head = __next;                     \
        } else {                                        \
            __prev->link.next = __next;                 \
        }                                               \
    } while(0);

#define ENQUEUE_THREAD_PRIO(thread, queue, link)       \
    do {                                               \
        OSThread* __prev;                              \
        OSThread* __next;                              \
        for(__next = (queue)->head; __next             \
          && (__next->priority <= (thread)->priority); \
                __next = __next->link.next) ;          \
                                                       \
        if (__next == NULL) {                          \
            ENQUEUE_THREAD(thread, queue, link);       \
        } else {                                       \
            (thread)->link.next = __next;              \
            __prev = __next->link.prev;                \
            __next->link.prev = (thread);              \
            (thread)->link.prev = __prev;              \
            if (__prev == NULL) {                      \
                (queue)->head = (thread);              \
            } else {                                   \
                __prev->link.next = (thread);          \
            }                                          \
        }                                              \
    } while(0);

#define DEQUEUE_HEAD(thread, queue, link)             \
    do {                                              \
        OSThread* __next = thread->link.next;         \
        if (__next == NULL) {                         \
            (queue)->tail = 0;                        \
        } else {                                      \
            __next->link.prev = 0;                    \
        }                                             \
        (queue)->head = __next;                       \
    } while(0);

/* Linker-defined symbols */
extern u8 _stack_end[];
extern u8 _stack_addr[];

/* Memory-mapped pointers */
#define OS_CURRENT_FPU_CONTEXT  (*(OSContext* volatile*)0x800000D8)
#define OS_CURRENT_THREAD       (*(OSThread* volatile*)0x800000E4)

/* Global active thread queue at low memory */
static OSThreadQueue* ActiveThreadQueue = (OSThreadQueue*)0x800000DC;

/* .bss */
static OSThreadQueue RunQueue[32];
static OSThread IdleThread;
static OSThread DefaultThread;
static OSContext IdleContext;
static volatile u32 RunQueueBits;
static volatile int RunQueueHint;
static s32 Reschedule;

static OSSwitchThreadCallback SwitchThreadCallback;

/* Forward declarations */
void OSInitThreadQueue(OSThreadQueue* queue);
static void __OSSwitchThread(OSThread* nextThread);
static void SetRun(OSThread* thread);
static void UnsetRun(OSThread* thread);
static OSThread* SetEffectivePriority(OSThread* thread, s32 priority);
static void UpdatePriority(OSThread* thread);
static OSThread* SelectThread(int yield);

/* External functions */
extern void __OSUnlockAllMutex(OSThread* thread);

void __OSThreadInit(void) {
    OSThread* thread = &DefaultThread;
    int prio;

    thread->state = 2;
    thread->attr = 1;
    thread->priority = thread->base = 16;
    thread->suspend = 0;
    thread->val = (u32)-1;
    thread->mutex = NULL;

    OSInitThreadQueue(&thread->queueJoin);
    thread->queueMutex.head = thread->queueMutex.tail = NULL;

    OS_CURRENT_FPU_CONTEXT = &thread->context;
    OSClearContext(&thread->context);
    OSSetCurrentContext(&thread->context);
    thread->stackBase = (u32*)_stack_addr;
    thread->stackEnd = (u32*)_stack_end;
    *(u32*)thread->stackEnd = 0xDEADBABE;
    OS_CURRENT_THREAD = thread;
    RunQueueBits = 0;
    RunQueueHint = 0;

    for (prio = 0; prio <= 31; prio++) {
        OSInitThreadQueue(&RunQueue[prio]);
    }
    OSInitThreadQueue(ActiveThreadQueue);

    ENQUEUE_THREAD(thread, ActiveThreadQueue, linkActive);

    OSClearContext(&IdleContext);
    Reschedule = 0;
}

void OSInitThreadQueue(OSThreadQueue* queue) {
    queue->head = queue->tail = NULL;
}

OSThread* OSGetCurrentThread(void) {
    return OS_CURRENT_THREAD;
}

static void __OSSwitchThread(OSThread* nextThread) {
    OS_CURRENT_THREAD = nextThread;
    OSSetCurrentContext(&nextThread->context);
    OSLoadContext(&nextThread->context);
}

BOOL OSIsThreadSuspended(OSThread* thread) {
    if (thread->suspend > 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL OSIsThreadTerminated(OSThread* thread) {
    return (thread->state == 8 || thread->state == 0) ? TRUE : FALSE;
}

static int __OSIsThreadActive(OSThread* thread) {
    OSThread* active;

    if (thread->state == 0) {
        return 0;
    }

    for (active = ActiveThreadQueue->head; active; active = active->linkActive.next) {
        if (thread == active) {
            return 1;
        }
    }
    return 0;
}

s32 OSDisableScheduler(void) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = Reschedule;
    Reschedule = count + 1;
    OSRestoreInterrupts(enabled);
    return count;
}

s32 OSEnableScheduler(void) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = Reschedule;
    Reschedule = count - 1;
    OSRestoreInterrupts(enabled);
    return count;
}

static void SetRun(OSThread* thread) {
    thread->queue = &RunQueue[thread->priority];
    ENQUEUE_THREAD(thread, thread->queue, link);
    RunQueueBits |= 1 << (31 - thread->priority);
    RunQueueHint = 1;
}

static void UnsetRun(OSThread* thread) {
    OSThreadQueue* queue;

    queue = thread->queue;
    DEQUEUE_THREAD(thread, queue, link);

    if (!queue->head) {
        RunQueueBits &= ~(1 << (31 - thread->priority));
    }
    thread->queue = NULL;
}

s32 __OSGetEffectivePriority(OSThread* thread) {
    s32 priority = thread->base;
    OSMutex* mutex;

    for (mutex = thread->queueMutex.head; mutex; mutex = mutex->link.next) {
        OSThread* blocked = mutex->queue.head;
        if (blocked && blocked->priority < priority) {
            priority = blocked->priority;
        }
    }
    return priority;
}

static OSThread* SetEffectivePriority(OSThread* thread, s32 priority) {
    switch (thread->state) {
        case 1:
            UnsetRun(thread);
            thread->priority = priority;
            SetRun(thread);
            break;
        case 4:
            DEQUEUE_THREAD(thread, thread->queue, link);
            thread->priority = priority;
            ENQUEUE_THREAD_PRIO(thread, thread->queue, link);
            if (thread->mutex) {
                return thread->mutex->thread;
            }
            break;
        case 2:
            RunQueueHint = 1;
            thread->priority = priority;
            break;
    }
    return NULL;
}

static void UpdatePriority(OSThread* thread) {
    s32 priority;

    while (1) {
        if (thread->suspend > 0) {
            break;
        }
        priority = __OSGetEffectivePriority(thread);
        if (thread->priority == priority) {
            break;
        }
        thread = SetEffectivePriority(thread, priority);
        if (thread == NULL) {
            break;
        }
    }
}

void __OSPromoteThread(OSThread* thread, s32 priority) {
    while (1) {
        if (thread->suspend > 0 || thread->priority <= priority) {
            break;
        }
        thread = SetEffectivePriority(thread, priority);
        if (thread == NULL) {
            break;
        }
    }
}

static OSThread* SelectThread(int yield) {
    OSContext* currentContext;
    OSThread* currentThread;
    OSThread* nextThread;
    s32 priority;
    OSThreadQueue* queue;

    if (Reschedule > 0) {
        return NULL;
    }

    currentContext = OSGetCurrentContext();
    currentThread = OSGetCurrentThread();

    if (currentContext != &currentThread->context) {
        return NULL;
    }

    if (currentThread) {
        if (currentThread->state == 2) {
            if (yield == 0) {
                priority = __cntlzw(RunQueueBits);
                if (currentThread->priority <= priority) {
                    return NULL;
                }
            }
            currentThread->state = 1;
            SetRun(currentThread);
        }
        if (!(currentThread->context.state & 2) && (OSSaveContext(&currentThread->context) != 0)) {
            return NULL;
        }
    }

    OS_CURRENT_THREAD = NULL;

    if (RunQueueBits == 0) {
        OSSetCurrentContext(&IdleContext);
        do {
            OSEnableInterrupts();
            while (RunQueueBits == 0) ;
            OSDisableInterrupts();
        } while (RunQueueBits == 0);
        OSClearContext(&IdleContext);
    }

    RunQueueHint = 0;
    priority = __cntlzw(RunQueueBits);

    queue = &RunQueue[priority];
    nextThread = queue->head;

    DEQUEUE_HEAD(nextThread, queue, link);

    if (!queue->head) {
        RunQueueBits &= ~(1 << (31 - priority));
    }
    nextThread->queue = NULL;
    nextThread->state = 2;
    __OSSwitchThread(nextThread);
    return nextThread;
}

void __OSReschedule(void) {
    if (RunQueueHint != 0) {
        SelectThread(0);
    }
}

void OSYieldThread(void) {
    BOOL enabled = OSDisableInterrupts();

    SelectThread(1);
    OSRestoreInterrupts(enabled);
}

BOOL OSCreateThread(OSThread* thread, void* (*func)(void*), void* param,
                    void* stack, u32 stackSize, s32 priority, u16 attr) {
    BOOL enabled;
    u32 sp;

    if ((priority < 0) || (priority > 31)) {
        return FALSE;
    }

    thread->state = 1;
    thread->attr = attr & 1;
    thread->base = priority;
    thread->priority = priority;
    thread->suspend = 1;
    thread->val = (u32)-1;
    thread->mutex = NULL;
    OSInitThreadQueue(&thread->queueJoin);
    OSInitThreadQueue((OSThreadQueue*)&thread->queueMutex);
    sp = (u32)stack;
    sp &= ~7;
    sp -= 8;
    ((u32*)sp)[0] = 0;
    ((u32*)sp)[1] = 0;
    OSInitContext(&thread->context, (u32)func, sp);
    thread->context.lr = (u32)OSExitThread;
    thread->context.gpr[3] = (u32)param;
    thread->stackBase = (u32*)stack;
    thread->stackEnd = (u32*)((u32)stack - stackSize);
    *thread->stackEnd = 0xDEADBABE;

    enabled = OSDisableInterrupts();
    ENQUEUE_THREAD(thread, ActiveThreadQueue, linkActive);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

void OSExitThread(void* val) {
    BOOL enabled = OSDisableInterrupts();
    OSThread* currentThread = OSGetCurrentThread();

    OSClearContext(&currentThread->context);
    if (currentThread->attr & 1) {
        DEQUEUE_THREAD(currentThread, ActiveThreadQueue, linkActive);
        currentThread->state = 0;
    } else {
        currentThread->state = 8;
        currentThread->val = (u32)val;
    }
    __OSUnlockAllMutex(currentThread);
    OSWakeupThread(&currentThread->queueJoin);
    RunQueueHint = 1;
    if (RunQueueHint != 0) {
        SelectThread(0);
    }
    OSRestoreInterrupts(enabled);
}

void OSCancelThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();

    switch (thread->state) {
        case 1:
            if (thread->suspend <= 0) {
                UnsetRun(thread);
            }
            break;
        case 2:
            RunQueueHint = 1;
            break;
        case 4:
            DEQUEUE_THREAD(thread, thread->queue, link);
            thread->queue = NULL;
            if ((thread->suspend <= 0) && (thread->mutex)) {
                UpdatePriority(thread->mutex->thread);
            }
            break;
        default:
            OSRestoreInterrupts(enabled);
            return;
    }
    OSClearContext(&thread->context);
    if (thread->attr & 1) {
        DEQUEUE_THREAD(thread, ActiveThreadQueue, linkActive);
        thread->state = 0;
    } else {
        thread->state = 8;
    }
    __OSUnlockAllMutex(thread);
    OSWakeupThread(&thread->queueJoin);
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

BOOL OSJoinThread(OSThread* thread, void* val) {
    BOOL enabled = OSDisableInterrupts();

    if (!(thread->attr & 1) && (thread->state != 8) && (thread->queueJoin.head == NULL)) {
        OSSleepThread(&thread->queueJoin);
        if (__OSIsThreadActive(thread) == 0) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }
    }
    if (thread->state == 8) {
        if (val) {
            *(s32*)val = (s32)thread->val;
        }
        DEQUEUE_THREAD(thread, ActiveThreadQueue, linkActive);
        thread->state = 0;
        OSRestoreInterrupts(enabled);
        return TRUE;
    }
    OSRestoreInterrupts(enabled);
    return FALSE;
}

void OSDetachThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();

    thread->attr |= 1;
    if (thread->state == 8) {
        DEQUEUE_THREAD(thread, ActiveThreadQueue, linkActive);
        thread->state = 0;
    }
    OSWakeupThread(&thread->queueJoin);
    OSRestoreInterrupts(enabled);
}

s32 OSResumeThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();
    s32 suspendCount;

    suspendCount = thread->suspend--;
    if (thread->suspend < 0) {
        thread->suspend = 0;
    } else if (thread->suspend == 0) {
        switch (thread->state) {
            case 1:
                thread->priority = __OSGetEffectivePriority(thread);
                SetRun(thread);
                break;
            case 4:
                DEQUEUE_THREAD(thread, thread->queue, link);
                thread->priority = __OSGetEffectivePriority(thread);
                ENQUEUE_THREAD_PRIO(thread, thread->queue, link);
                if (thread->mutex) {
                    UpdatePriority(thread->mutex->thread);
                }
                break;
        }
        __OSReschedule();
    }
    OSRestoreInterrupts(enabled);
    return suspendCount;
}

s32 OSSuspendThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();
    s32 suspendCount;

    suspendCount = thread->suspend++;
    if (suspendCount == 0) {
        switch (thread->state) {
            case 2:
                RunQueueHint = 1;
                thread->state = 1;
                break;
            case 1:
                UnsetRun(thread);
                break;
            case 4:
                DEQUEUE_THREAD(thread, thread->queue, link);
                thread->priority = 32;
                ENQUEUE_THREAD(thread, thread->queue, link);
                if (thread->mutex) {
                    UpdatePriority(thread->mutex->thread);
                }
                break;
        }
        __OSReschedule();
    }
    OSRestoreInterrupts(enabled);
    return suspendCount;
}

void OSSleepThread(OSThreadQueue* queue) {
    BOOL enabled = OSDisableInterrupts();
    OSThread* currentThread = OSGetCurrentThread();

    currentThread->state = 4;
    currentThread->queue = queue;
    ENQUEUE_THREAD_PRIO(currentThread, queue, link);
    RunQueueHint = 1;
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

void OSWakeupThread(OSThreadQueue* queue) {
    BOOL enabled = OSDisableInterrupts();

    while (queue->head) {
        OSThread* thread = queue->head;

        DEQUEUE_HEAD(thread, queue, link);

        thread->state = 1;
        if (thread->suspend <= 0) {
            SetRun(thread);
        }
    }
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

BOOL OSSetThreadPriority(OSThread* thread, s32 priority) {
    BOOL enabled;

    if ((priority < 0) || (priority > 31)) {
        return FALSE;
    }
    enabled = OSDisableInterrupts();

    if (thread->base != priority) {
        thread->base = priority;
        UpdatePriority(thread);
        __OSReschedule();
    }
    OSRestoreInterrupts(enabled);
    return TRUE;
}

s32 OSGetThreadPriority(OSThread* thread) {
    return thread->base;
}

OSThread* OSSetIdleFunction(void (*idleFunction)(void*), void* param,
                            void* stack, u32 stackSize) {
    if (idleFunction) {
        if (IdleThread.state == 0) {
            OSCreateThread(&IdleThread, (void* (*)(void*))idleFunction, param,
                           stack, stackSize, 31, 1);
            OSResumeThread(&IdleThread);
            return &IdleThread;
        }
    } else if (IdleThread.state != 0) {
        OSCancelThread(&IdleThread);
    }
    return NULL;
}

OSThread* OSGetIdleFunction(void) {
    if (IdleThread.state != 0) {
        return &IdleThread;
    }
    return NULL;
}

void OSClearStack(u8 val) {
    u32 sp;
    u32* stackEnd;
    u32 pattern;
    u32* p;

    pattern = (val << 24) | (val << 16) | (val << 8) | val;
    sp = (u32)&sp;
    stackEnd = OS_CURRENT_THREAD->stackEnd;
    stackEnd += 1;

    if ((u32)stackEnd >= sp) {
        return;
    }

    p = stackEnd;
    while ((u32)p < sp) {
        *p++ = pattern;
    }
}

static int CheckThreadQueue(OSThreadQueue* queue) {
    OSThread* thread;

    if ((queue->head != NULL) && (queue->head->link.prev != NULL)) {
        return 0;
    }
    if ((queue->tail != NULL) && (queue->tail->link.next != NULL)) {
        return 0;
    }
    thread = queue->head;
    while (thread) {
        if ((thread->link.next != NULL) && (thread != thread->link.next->link.prev)) {
            return 0;
        }
        if ((thread->link.prev != NULL) && (thread != thread->link.prev->link.next)) {
            return 0;
        }
        thread = thread->link.next;
    }
    return 1;
}

static int IsMember(OSThreadQueue* queue, OSThread* thread) {
    OSThread* member = queue->head;

    while (member) {
        if (thread == member) {
            return 1;
        }
        member = member->link.next;
    }
    return 0;
}

s32 OSCheckActiveThreads(void) {
    OSThread* thread;
    s32 prio;
    s32 cThread;
    BOOL enabled;

    cThread = 0;
    enabled = OSDisableInterrupts();

    for (prio = 0; prio <= 31; prio++) {
        if (RunQueueBits & (1 << (31 - prio))) {
        } else {
        }
    }

    thread = ActiveThreadQueue->head;
    while (thread) {
        cThread++;
        switch (thread->state) {
            case 1:
                break;
            case 2:
                break;
            case 4:
                break;
            case 8:
                break;
        }
        thread = thread->linkActive.next;
    }
    OSRestoreInterrupts(enabled);
    return cThread;
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800A13F8 - 0x800A13F8 | size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A13F8(void) {
    u32 r3 = 0;

    r3 = (0x8000 << 16);
    r3 = *(u32*)((u8*)r3 + 0xE4);
    return;
}
#pragma pop

/* fn_800A1484 - 0x800A1484 | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A1484(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = *(u32*)((u8*)r3 + 0x2E0);
    r5 = *(u32*)((u8*)r3 + 0x2DC);
    r6 = *(u32*)((u8*)r3 + 0x2E4);
    if ((u32)r4 != (u32)0x0) goto L_800A14A0;
    *(u32*)((u8*)r5 + 0x4) = r6;
    goto L_800A14A4;
L_800A14A0: ;
    *(u32*)((u8*)r4 + 0x2E4) = r6;
L_800A14A4: ;
    if ((u32)r6 != (u32)0x0) goto L_800A14B4;
    *(u32*)((u8*)r5 + 0x0) = r4;
    goto L_800A14B8;
L_800A14B4: ;
    *(u32*)((u8*)r6 + 0x2E0) = r4;
L_800A14B8: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_800A14E0;
    r0 = *(u32*)((u8*)r3 + 0x2D0);
    r4 = 0x1;
    r5 = *(u32*)RunQueueBits_8047A760;
    r0 = 0x1f - r0;
    r0 = r4 << r0;
    r0 = r5 & ~r0;
    *(u32*)RunQueueBits_8047A760 = r0;
L_800A14E0: ;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x2DC) = r0;
    return;
}
#pragma pop

/* fn_800A14EC - 0x800A14EC | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A14EC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f4 = 0.0f;

    r4 = *(u32*)((u8*)r3 + 0x2D4);
    r5 = *(u32*)((u8*)r3 + 0x2F4);
    goto L_800A1518;
L_800A14F8: ;
    r3 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_800A1514;
    r0 = *(u32*)((u8*)r3 + 0x2D0);
    if ((s32)r0 >= (s32)r4) goto L_800A1514;
    r4 = r0;
L_800A1514: ;
    r5 = *(u32*)((u8*)r5 + 0x10);
L_800A1518: ;
    if ((u32)r5 != (u32)0x0) goto L_800A14F8;
    r3 = r4;
    return;
}
#pragma pop

/* fn_800A1528 - 0x800A1528 | size: 0x1C0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A1528(void) {
    extern void fn_800A1484();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r31 = r3;
    r30 = r4 + 0x0;
    r0 = *(u16*)((u8*)r3 + 0x2C8);
    if ((s32)r0 == (s32)0x3) goto L_800A16CC;
    if ((s32)r0 >= (s32)0x3) goto L_800A1564;
    if ((s32)r0 == (s32)0x1) goto L_800A1570;
    if ((s32)r0 >= (s32)0x1) goto L_800A16C0;
    goto L_800A16CC;
L_800A1564: ;
    if ((s32)r0 >= (s32)0x5) goto L_800A16CC;
    goto L_800A15E8;
L_800A1570: ;
    r3 = r31;
    fn_800A1484();
    *(u32*)((u8*)r31 + 0x2D0) = r30;
    r3 = (u32)RunQueue_803FB898;
    r0 = (u32)RunQueue_803FB898;
    r3 = *(u32*)((u8*)r31 + 0x2D0);
    r3 = r3 << 3;
    r0 = r0 + r3;
    *(u32*)((u8*)r31 + 0x2DC) = r0;
    r4 = *(u32*)((u8*)r31 + 0x2DC);
    r3 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r3 != (u32)0x0) goto L_800A15AC;
    *(u32*)((u8*)r4 + 0x0) = r31;
    goto L_800A15B0;
L_800A15AC: ;
    *(u32*)((u8*)r3 + 0x2E0) = r31;
L_800A15B0: ;
    *(u32*)((u8*)r31 + 0x2E4) = r3;
    r0 = 0x0;
    r3 = 0x1;
    *(u32*)((u8*)r31 + 0x2E0) = r0;
    r4 = *(u32*)((u8*)r31 + 0x2DC);
    *(u32*)((u8*)r4 + 0x4) = r31;
    r0 = *(u32*)((u8*)r31 + 0x2D0);
    r4 = *(u32*)RunQueueBits_8047A760;
    r0 = 0x1f - r0;
    r0 = r3 << r0;
    r0 = r4 | r0;
    *(u32*)RunQueueBits_8047A760 = r0;
    *(u32*)RunQueueHint_8047A764 = r3;
    goto L_800A16CC;
L_800A15E8: ;
    r4 = *(u32*)((u8*)r31 + 0x2E0);
    r5 = *(u32*)((u8*)r31 + 0x2E4);
    if ((u32)r4 != (u32)0x0) goto L_800A1604;
    r3 = *(u32*)((u8*)r31 + 0x2DC);
    *(u32*)((u8*)r3 + 0x4) = r5;
    goto L_800A1608;
L_800A1604: ;
    *(u32*)((u8*)r4 + 0x2E4) = r5;
L_800A1608: ;
    if ((u32)r5 != (u32)0x0) goto L_800A161C;
    r3 = *(u32*)((u8*)r31 + 0x2DC);
    *(u32*)((u8*)r3 + 0x0) = r4;
    goto L_800A1620;
L_800A161C: ;
    *(u32*)((u8*)r5 + 0x2E0) = r4;
L_800A1620: ;
    *(u32*)((u8*)r31 + 0x2D0) = r30;
    r4 = *(u32*)((u8*)r31 + 0x2DC);
    r5 = *(u32*)((u8*)r4 + 0x0);
    goto L_800A1634;
L_800A1630: ;
    r5 = *(u32*)((u8*)r5 + 0x2E0);
L_800A1634: ;
    if ((u32)r5 == (u32)0x0) goto L_800A164C;
    r3 = *(u32*)((u8*)r5 + 0x2D0);
    r0 = *(u32*)((u8*)r31 + 0x2D0);
    if ((s32)r3 <= (s32)r0) goto L_800A1630;
L_800A164C: ;
    if ((u32)r5 != (u32)0x0) goto L_800A1684;
    r3 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r3 != (u32)0x0) goto L_800A1668;
    *(u32*)((u8*)r4 + 0x0) = r31;
    goto L_800A166C;
L_800A1668: ;
    *(u32*)((u8*)r3 + 0x2E0) = r31;
L_800A166C: ;
    *(u32*)((u8*)r31 + 0x2E4) = r3;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2E0) = r0;
    r3 = *(u32*)((u8*)r31 + 0x2DC);
    *(u32*)((u8*)r3 + 0x4) = r31;
    goto L_800A16AC;
L_800A1684: ;
    *(u32*)((u8*)r31 + 0x2E0) = r5;
    r3 = *(u32*)((u8*)r5 + 0x2E4);
    *(u32*)((u8*)r5 + 0x2E4) = r31;
    *(u32*)((u8*)r31 + 0x2E4) = r3;
    if ((u32)r3 != (u32)0x0) goto L_800A16A8;
    r3 = *(u32*)((u8*)r31 + 0x2DC);
    *(u32*)((u8*)r3 + 0x0) = r31;
    goto L_800A16AC;
L_800A16A8: ;
    *(u32*)((u8*)r3 + 0x2E0) = r31;
L_800A16AC: ;
    r3 = *(u32*)((u8*)r31 + 0x2F0);
    if ((u32)r3 == (u32)0x0) goto L_800A16CC;
    r3 = *(u32*)((u8*)r3 + 0x8);
    goto L_800A16D0;
L_800A16C0: ;
    r0 = 0x1;
    *(u32*)RunQueueHint_8047A764 = r0;
    *(u32*)((u8*)r31 + 0x2D0) = r30;
L_800A16CC: ;
    r3 = 0x0;
L_800A16D0: ;
    return;
}
#pragma pop

/* fn_800A16E8 - 0x800A16E8 | size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A16E8(void) {
    extern void fn_800A1528();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r4;
L_800A16FC: ;
    r0 = *(u32*)((u8*)r3 + 0x2CC);
    if ((s32)r0 > (s32)0x0) goto L_800A1724;
    r0 = *(u32*)((u8*)r3 + 0x2D0);
    if ((s32)r0 <= (s32)r31) goto L_800A1724;
    r4 = r31;
    fn_800A1528();
    if ((u32)r3 != (u32)0x0) goto L_800A16FC;
L_800A1724: ;
    return;
}
#pragma pop

/* fn_800A1990 - 0x800A1990 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A1990(void) {
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    OSDisableInterrupts();
    r31 = r3 + 0x0;
    r3 = 0x1;
    SelectThread();
    r3 = r31;
    OSRestoreInterrupts();
    return;
}
#pragma pop

/* fn_800A19CC - 0x800A19CC | size: 0x1E8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A19CC(void) {
    extern u8 lbl_80478990[];
    extern void fn_8009BD84();
    extern void fn_800A1BB4();
    u32 r0 = 0;
    u32 r1 = 0;
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
    f32 f4 = 0.0f;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r27, 0x3c(r1) */;
    r31 = r3 + 0x0;
    r27 = r5 + 0x0;
    r28 = r6 + 0x0;
    r29 = r7 + 0x0;
    if ((s32)r8 < (s32)0x0) goto L_800A19FC;
    if ((s32)r8 <= (s32)0x1f) goto L_800A1A04;
L_800A19FC: ;
    r3 = 0x0;
    goto L_800A1BA0;
L_800A1A04: ;
    r6 = 0x1;
    *(u16*)((u8*)r31 + 0x2C8) = r6;
    r0 = r9 & 0x1;
    /* clrrwi r7, r28, 3 */;
    *(u16*)((u8*)r31 + 0x2CA) = r0;
    r0 = -0x1;
    r30 = 0x0;
    *(u32*)((u8*)r31 + 0x2D4) = r8;
    r3 = r31 + 0x0;
    /* subi r5, r7, 0x8 */;
    *(u32*)((u8*)r31 + 0x2D0) = r8;
    *(u32*)((u8*)r31 + 0x2CC) = r6;
    *(u32*)((u8*)r31 + 0x2D8) = r0;
    *(u32*)((u8*)r31 + 0x2F0) = r30;
    *(u32*)((u8*)r31 + 0x2EC) = r30;
    *(u32*)((u8*)r31 + 0x2E8) = r30;
    *(u32*)((u8*)r31 + 0x2F8) = r30;
    *(u32*)((u8*)r31 + 0x2F4) = r30;
    *(u32*)((u8*)r7 + (-8)) = r30;
    *(u32*)((u8*)r7 + (-4)) = r30;
    fn_8009BD84();
    r3 = (u32)fn_800A1BB4;
    r0 = (u32)fn_800A1BB4;
    *(u32*)((u8*)r31 + 0x84) = r0;
    r3 = (0xdeae << 16);
    r4 = r28 - r29;
    *(u32*)((u8*)r31 + 0xC) = r27;
    /* subi r0, r3, 0x4542 */;
    *(u32*)((u8*)r31 + 0x304) = r28;
    *(u32*)((u8*)r31 + 0x308) = r4;
    r3 = *(u32*)((u8*)r31 + 0x308);
    *(u32*)((u8*)r3 + 0x0) = r0;
    *(u32*)((u8*)r31 + 0x30C) = r30;
    *(u32*)((u8*)r31 + 0x310) = r30;
    *(u32*)((u8*)r31 + 0x314) = r30;
    OSDisableInterrupts();
    r4 = (u32)__OSErrorTable;
    r4 = (u32)__OSErrorTable;
    r0 = *(u32*)((u8*)r4 + 0x40);
    if ((u32)r0 == (u32)0x0) goto L_800A1B68;
    r4 = *(u32*)((u8*)r31 + 0x19C);
    r0 = 0x4;
    ctr_fn = (void(*)(void))r0;
    r5 = r31 + 0x0;
    r0 = r4 | 0x900;
    *(u32*)((u8*)r31 + 0x19C) = r0;
    r0 = *(u16*)((u8*)r31 + 0x1A2);
    r0 = r0 | 0x1;
    *(u16*)((u8*)r31 + 0x1A2) = r0;
    r0 = *(u32*)lbl_80478990;
    r0 = r0 & 0x000000F8;
    r0 = r0 | 0x4;
    *(u32*)((u8*)r31 + 0x194) = r0;
L_800A1ADC: ;
    r0 = -0x1;
    *(u32*)((u8*)r5 + 0x94) = r0;
    *(u32*)((u8*)r5 + 0x90) = r0;
    *(u32*)((u8*)r5 + 0x1CC) = r0;
    *(u32*)((u8*)r5 + 0x1C8) = r0;
    *(u32*)((u8*)r5 + 0x9C) = r0;
    *(u32*)((u8*)r5 + 0x98) = r0;
    *(u32*)((u8*)r5 + 0x1D4) = r0;
    *(u32*)((u8*)r5 + 0x1D0) = r0;
    *(u32*)((u8*)r5 + 0xA4) = r0;
    *(u32*)((u8*)r5 + 0xA0) = r0;
    *(u32*)((u8*)r5 + 0x1DC) = r0;
    *(u32*)((u8*)r5 + 0x1D8) = r0;
    *(u32*)((u8*)r5 + 0xAC) = r0;
    *(u32*)((u8*)r5 + 0xA8) = r0;
    *(u32*)((u8*)r5 + 0x1E4) = r0;
    *(u32*)((u8*)r5 + 0x1E0) = r0;
    *(u32*)((u8*)r5 + 0xB4) = r0;
    *(u32*)((u8*)r5 + 0xB0) = r0;
    *(u32*)((u8*)r5 + 0x1EC) = r0;
    *(u32*)((u8*)r5 + 0x1E8) = r0;
    *(u32*)((u8*)r5 + 0xBC) = r0;
    *(u32*)((u8*)r5 + 0xB8) = r0;
    *(u32*)((u8*)r5 + 0x1F4) = r0;
    *(u32*)((u8*)r5 + 0x1F0) = r0;
    *(u32*)((u8*)r5 + 0xC4) = r0;
    *(u32*)((u8*)r5 + 0xC0) = r0;
    *(u32*)((u8*)r5 + 0x1FC) = r0;
    *(u32*)((u8*)r5 + 0x1F8) = r0;
    *(u32*)((u8*)r5 + 0xCC) = r0;
    *(u32*)((u8*)r5 + 0xC8) = r0;
    *(u32*)((u8*)r5 + 0x204) = r0;
    *(u32*)((u8*)r5 + 0x200) = r0;
    r5 = r5 + 0x40;
    if (--ctr != 0) goto L_800A1ADC;
L_800A1B68: ;
    r4 = (0x8000 << 16);
    r5 = r4 + 0xdc;
    r6 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r6 != (u32)0x0) goto L_800A1B84;
    *(u32*)((u8*)r4 + 0xDC) = r31;
    goto L_800A1B88;
L_800A1B84: ;
    *(u32*)((u8*)r6 + 0x2FC) = r31;
L_800A1B88: ;
    *(u32*)((u8*)r31 + 0x300) = r6;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2FC) = r0;
    *(u32*)((u8*)r5 + 0x0) = r31;
    OSRestoreInterrupts();
    r3 = 0x1;
L_800A1BA0: ;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* fn_800A1BB4 - 0x800A1BB4 | size: 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A1BB4(void) {
    extern void fn_8009F958();
    extern void fn_800A2478();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    OSDisableInterrupts();
    r31 = (0x8000 << 16);
    r30 = *(u32*)((u8*)r31 + 0xE4);
    r29 = r3 + 0x0;
    r3 = r30 + 0x0;
    OSClearContext();
    r0 = *(u16*)((u8*)r30 + 0x2CA);
    r0 = r0 & 0x1;
    if ((s32)r0 == (s32)0) goto L_800A1C38;
    r4 = *(u32*)((u8*)r30 + 0x2FC);
    r5 = *(u32*)((u8*)r30 + 0x300);
    if ((u32)r4 != (u32)0x0) goto L_800A1C10;
    *(u32*)((u8*)r31 + 0xE0) = r5;
    goto L_800A1C14;
L_800A1C10: ;
    *(u32*)((u8*)r4 + 0x300) = r5;
L_800A1C14: ;
    if ((u32)r5 != (u32)0x0) goto L_800A1C28;
    r3 = (0x8000 << 16);
    *(u32*)((u8*)r3 + 0xDC) = r4;
    goto L_800A1C2C;
L_800A1C28: ;
    *(u32*)((u8*)r5 + 0x2FC) = r4;
L_800A1C2C: ;
    r0 = 0x0;
    *(u16*)((u8*)r30 + 0x2C8) = r0;
    goto L_800A1C44;
L_800A1C38: ;
    r0 = 0x8;
    *(u16*)((u8*)r30 + 0x2C8) = r0;
    *(u32*)((u8*)r30 + 0x2D8) = r28;
L_800A1C44: ;
    r3 = r30;
    fn_8009F958();
    r3 = r30 + 0x2e8;
    fn_800A2478();
    r0 = 0x1;
    *(u32*)RunQueueHint_8047A764 = r0;
    r0 = *(u32*)RunQueueHint_8047A764;
    if ((s32)r0 == (s32)0x0) goto L_800A1C70;
    r3 = 0x0;
    SelectThread();
L_800A1C70: ;
    r3 = r29;
    OSRestoreInterrupts();
    return;
}
#pragma pop

/* fn_800A1E54 - 0x800A1E54 | size: 0x140 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A1E54(void) {
    extern void fn_800A238C();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3 + 0x0;
    r29 = r4 + 0x0;
    OSDisableInterrupts();
    r0 = *(u16*)((u8*)r31 + 0x2CA);
    r30 = r3 + 0x0;
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)0) goto L_800A1F00;
    r0 = *(u16*)((u8*)r31 + 0x2C8);
    if ((u32)r0 == (u32)0x8) goto L_800A1F00;
    r0 = *(u32*)((u8*)r31 + 0x2E8);
    if ((u32)r0 != (u32)0x0) goto L_800A1F00;
    r3 = r31 + 0x2e8;
    fn_800A238C();
    r0 = *(u16*)((u8*)r31 + 0x2C8);
    if ((u32)r0 != (u32)0x0) goto L_800A1EBC;
    r0 = 0x0;
    goto L_800A1EE8;
L_800A1EBC: ;
    r3 = (0x8000 << 16);
    r3 = *(u32*)((u8*)r3 + 0xDC);
    goto L_800A1EDC;
L_800A1EC8: ;
    if ((u32)r31 != (u32)r3) goto L_800A1ED8;
    r0 = 0x1;
    goto L_800A1EE8;
L_800A1ED8: ;
    r3 = *(u32*)((u8*)r3 + 0x2FC);
L_800A1EDC: ;
    if ((u32)r3 != (u32)0x0) goto L_800A1EC8;
    r0 = 0x0;
L_800A1EE8: ;
    if ((s32)r0 != (s32)0x0) goto L_800A1F00;
    r3 = r30;
    OSRestoreInterrupts();
    r3 = 0x0;
    goto L_800A1F78;
L_800A1F00: ;
    r0 = *(u16*)((u8*)r31 + 0x2C8);
    if ((u32)r0 != (u32)0x8) goto L_800A1F6C;
    if ((u32)r29 == (u32)0x0) goto L_800A1F1C;
    r0 = *(u32*)((u8*)r31 + 0x2D8);
    *(u32*)((u8*)r29 + 0x0) = r0;
L_800A1F1C: ;
    r4 = *(u32*)((u8*)r31 + 0x2FC);
    r5 = *(u32*)((u8*)r31 + 0x300);
    if ((u32)r4 != (u32)0x0) goto L_800A1F38;
    r3 = (0x8000 << 16);
    *(u32*)((u8*)r3 + 0xE0) = r5;
    goto L_800A1F3C;
L_800A1F38: ;
    *(u32*)((u8*)r4 + 0x300) = r5;
L_800A1F3C: ;
    if ((u32)r5 != (u32)0x0) goto L_800A1F50;
    r3 = (0x8000 << 16);
    *(u32*)((u8*)r3 + 0xDC) = r4;
    goto L_800A1F54;
L_800A1F50: ;
    *(u32*)((u8*)r5 + 0x2FC) = r4;
L_800A1F54: ;
    r0 = 0x0;
    *(u16*)((u8*)r31 + 0x2C8) = r0;
    r3 = r30;
    OSRestoreInterrupts();
    r3 = 0x1;
    goto L_800A1F78;
L_800A1F6C: ;
    r3 = r30;
    OSRestoreInterrupts();
    r3 = 0x0;
L_800A1F78: ;
    return;
}
#pragma pop

/* fn_800A1F94 - 0x800A1F94 | size: 0x288 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A1F94(void) {
    extern void fn_800A14EC();
    extern void fn_800A1528();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;

    r29 = r3;
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r29 + 0x2CC);
    r31 = r3 + 0x0;
    /* subi r0, r4, 0x1 */;
    *(u32*)((u8*)r29 + 0x2CC) = r0;
    r30 = r4;
    r0 = *(u32*)((u8*)r29 + 0x2CC);
    if ((s32)r0 >= (s32)0x0) goto L_800A1FE0;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x2CC) = r0;
    goto L_800A21F4;
L_800A1FE0: ;
    if ((s32)r0 != (s32)0x0) goto L_800A21F4;
    r0 = *(u16*)((u8*)r29 + 0x2C8);
    if ((s32)r0 == (s32)0x4) goto L_800A20A4;
    if ((s32)r0 >= (s32)0x4) goto L_800A21E0;
    if ((s32)r0 == (s32)0x1) goto L_800A2000;
    goto L_800A21E0;
L_800A2000: ;
    r0 = *(u32*)((u8*)r29 + 0x2D4);
    r3 = *(u32*)((u8*)r29 + 0x2F4);
    goto L_800A202C;
L_800A200C: ;
    r4 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r4 == (u32)0x0) goto L_800A2028;
    r4 = *(u32*)((u8*)r4 + 0x2D0);
    if ((s32)r4 >= (s32)r0) goto L_800A2028;
    r0 = r4;
L_800A2028: ;
    r3 = *(u32*)((u8*)r3 + 0x10);
L_800A202C: ;
    if ((u32)r3 != (u32)0x0) goto L_800A200C;
    *(u32*)((u8*)r29 + 0x2D0) = r0;
    r3 = (u32)RunQueue_803FB898;
    r0 = (u32)RunQueue_803FB898;
    r3 = *(u32*)((u8*)r29 + 0x2D0);
    r3 = r3 << 3;
    r0 = r0 + r3;
    *(u32*)((u8*)r29 + 0x2DC) = r0;
    r4 = *(u32*)((u8*)r29 + 0x2DC);
    r3 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r3 != (u32)0x0) goto L_800A2068;
    *(u32*)((u8*)r4 + 0x0) = r29;
    goto L_800A206C;
L_800A2068: ;
    *(u32*)((u8*)r3 + 0x2E0) = r29;
L_800A206C: ;
    *(u32*)((u8*)r29 + 0x2E4) = r3;
    r0 = 0x0;
    r3 = 0x1;
    *(u32*)((u8*)r29 + 0x2E0) = r0;
    r4 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r4 + 0x4) = r29;
    r0 = *(u32*)((u8*)r29 + 0x2D0);
    r4 = *(u32*)RunQueueBits_8047A760;
    r0 = 0x1f - r0;
    r0 = r3 << r0;
    r0 = r4 | r0;
    *(u32*)RunQueueBits_8047A760 = r0;
    *(u32*)RunQueueHint_8047A764 = r3;
    goto L_800A21E0;
L_800A20A4: ;
    r4 = *(u32*)((u8*)r29 + 0x2E0);
    r5 = *(u32*)((u8*)r29 + 0x2E4);
    if ((u32)r4 != (u32)0x0) goto L_800A20C0;
    r3 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r3 + 0x4) = r5;
    goto L_800A20C4;
L_800A20C0: ;
    *(u32*)((u8*)r4 + 0x2E4) = r5;
L_800A20C4: ;
    if ((u32)r5 != (u32)0x0) goto L_800A20D8;
    r3 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r3 + 0x0) = r4;
    goto L_800A20DC;
L_800A20D8: ;
    *(u32*)((u8*)r5 + 0x2E0) = r4;
L_800A20DC: ;
    r0 = *(u32*)((u8*)r29 + 0x2D4);
    r3 = *(u32*)((u8*)r29 + 0x2F4);
    goto L_800A2108;
L_800A20E8: ;
    r4 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r4 == (u32)0x0) goto L_800A2104;
    r4 = *(u32*)((u8*)r4 + 0x2D0);
    if ((s32)r4 >= (s32)r0) goto L_800A2104;
    r0 = r4;
L_800A2104: ;
    r3 = *(u32*)((u8*)r3 + 0x10);
L_800A2108: ;
    if ((u32)r3 != (u32)0x0) goto L_800A20E8;
    *(u32*)((u8*)r29 + 0x2D0) = r0;
    r4 = *(u32*)((u8*)r29 + 0x2DC);
    r5 = *(u32*)((u8*)r4 + 0x0);
    goto L_800A2124;
L_800A2120: ;
    r5 = *(u32*)((u8*)r5 + 0x2E0);
L_800A2124: ;
    if ((u32)r5 == (u32)0x0) goto L_800A213C;
    r3 = *(u32*)((u8*)r5 + 0x2D0);
    r0 = *(u32*)((u8*)r29 + 0x2D0);
    if ((s32)r3 <= (s32)r0) goto L_800A2120;
L_800A213C: ;
    if ((u32)r5 != (u32)0x0) goto L_800A2174;
    r3 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r3 != (u32)0x0) goto L_800A2158;
    *(u32*)((u8*)r4 + 0x0) = r29;
    goto L_800A215C;
L_800A2158: ;
    *(u32*)((u8*)r3 + 0x2E0) = r29;
L_800A215C: ;
    *(u32*)((u8*)r29 + 0x2E4) = r3;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x2E0) = r0;
    r3 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r3 + 0x4) = r29;
    goto L_800A219C;
L_800A2174: ;
    *(u32*)((u8*)r29 + 0x2E0) = r5;
    r3 = *(u32*)((u8*)r5 + 0x2E4);
    *(u32*)((u8*)r5 + 0x2E4) = r29;
    *(u32*)((u8*)r29 + 0x2E4) = r3;
    if ((u32)r3 != (u32)0x0) goto L_800A2198;
    r3 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r3 + 0x0) = r29;
    goto L_800A219C;
L_800A2198: ;
    *(u32*)((u8*)r3 + 0x2E0) = r29;
L_800A219C: ;
    r3 = *(u32*)((u8*)r29 + 0x2F0);
    if ((u32)r3 == (u32)0x0) goto L_800A21E0;
    r29 = *(u32*)((u8*)r3 + 0x8);
L_800A21AC: ;
    r0 = *(u32*)((u8*)r29 + 0x2CC);
    if ((s32)r0 > (s32)0x0) goto L_800A21E0;
    r3 = r29;
    fn_800A14EC();
    r0 = *(u32*)((u8*)r29 + 0x2D0);
    r4 = r3 + 0x0;
    if ((s32)r0 == (s32)r4) goto L_800A21E0;
    r3 = r29;
    fn_800A1528();
    /* mr. r29, r3 */;
    if ((s32)r0 != (s32)r4) goto L_800A21AC;
L_800A21E0: ;
    r0 = *(u32*)RunQueueHint_8047A764;
    if ((s32)r0 == (s32)0x0) goto L_800A21F4;
    r3 = 0x0;
    SelectThread();
L_800A21F4: ;
    r3 = r31;
    OSRestoreInterrupts();
    r3 = r30;
    return;
}
#pragma pop

/* fn_800A221C - 0x800A221C | size: 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A221C(void) {
    extern void fn_800A1484();
    extern void fn_800A14EC();
    extern void fn_800A1528();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r29 = r3;
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r29 + 0x2CC);
    r31 = r3 + 0x0;
    r0 = r4 + 0x1;
    /* mr. r30, r4 */;
    *(u32*)((u8*)r29 + 0x2CC) = r0;
    if ((s32)r0 != (s32)0) goto L_800A2364;
    r0 = *(u16*)((u8*)r29 + 0x2C8);
    if ((s32)r0 == (s32)0x3) goto L_800A2350;
    if ((s32)r0 >= (s32)0x3) goto L_800A2274;
    if ((s32)r0 == (s32)0x1) goto L_800A2290;
    if ((s32)r0 >= (s32)0x1) goto L_800A2280;
    goto L_800A2350;
L_800A2274: ;
    if ((s32)r0 >= (s32)0x5) goto L_800A2350;
    goto L_800A229C;
L_800A2280: ;
    r0 = 0x1;
    *(u32*)RunQueueHint_8047A764 = r0;
    *(u16*)((u8*)r29 + 0x2C8) = r0;
    goto L_800A2350;
L_800A2290: ;
    r3 = r29;
    fn_800A1484();
    goto L_800A2350;
L_800A229C: ;
    r4 = *(u32*)((u8*)r29 + 0x2E0);
    r5 = *(u32*)((u8*)r29 + 0x2E4);
    if ((u32)r4 != (u32)0x0) goto L_800A22B8;
    r3 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r3 + 0x4) = r5;
    goto L_800A22BC;
L_800A22B8: ;
    *(u32*)((u8*)r4 + 0x2E4) = r5;
L_800A22BC: ;
    if ((u32)r5 != (u32)0x0) goto L_800A22D0;
    r3 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r3 + 0x0) = r4;
    goto L_800A22D4;
L_800A22D0: ;
    *(u32*)((u8*)r5 + 0x2E0) = r4;
L_800A22D4: ;
    r0 = 0x20;
    *(u32*)((u8*)r29 + 0x2D0) = r0;
    r4 = *(u32*)((u8*)r29 + 0x2DC);
    r3 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r3 != (u32)0x0) goto L_800A22F4;
    *(u32*)((u8*)r4 + 0x0) = r29;
    goto L_800A22F8;
L_800A22F4: ;
    *(u32*)((u8*)r3 + 0x2E0) = r29;
L_800A22F8: ;
    *(u32*)((u8*)r29 + 0x2E4) = r3;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x2E0) = r0;
    r3 = *(u32*)((u8*)r29 + 0x2DC);
    *(u32*)((u8*)r3 + 0x4) = r29;
    r3 = *(u32*)((u8*)r29 + 0x2F0);
    if ((u32)r3 == (u32)0x0) goto L_800A2350;
    r29 = *(u32*)((u8*)r3 + 0x8);
L_800A231C: ;
    r0 = *(u32*)((u8*)r29 + 0x2CC);
    if ((s32)r0 > (s32)0x0) goto L_800A2350;
    r3 = r29;
    fn_800A14EC();
    r0 = *(u32*)((u8*)r29 + 0x2D0);
    r4 = r3 + 0x0;
    if ((s32)r0 == (s32)r4) goto L_800A2350;
    r3 = r29;
    fn_800A1528();
    /* mr. r29, r3 */;
    if ((s32)r0 != (s32)r4) goto L_800A231C;
L_800A2350: ;
    r0 = *(u32*)RunQueueHint_8047A764;
    if ((s32)r0 == (s32)0x0) goto L_800A2364;
    r3 = 0x0;
    SelectThread();
L_800A2364: ;
    r3 = r31;
    OSRestoreInterrupts();
    r3 = r30;
    return;
}
#pragma pop

/* fn_800A238C - 0x800A238C | size: 0xEC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A238C(void) {
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    OSDisableInterrupts();
    r4 = (0x8000 << 16);
    r4 = *(u32*)((u8*)r4 + 0xE4);
    r0 = 0x4;
    r31 = r3;
    *(u16*)((u8*)r4 + 0x2C8) = r0;
    *(u32*)((u8*)r4 + 0x2DC) = r30;
    r5 = *(u32*)((u8*)r30 + 0x0);
    goto L_800A23CC;
L_800A23C8: ;
    r5 = *(u32*)((u8*)r5 + 0x2E0);
L_800A23CC: ;
    if ((u32)r5 == (u32)0x0) goto L_800A23E4;
    r3 = *(u32*)((u8*)r5 + 0x2D0);
    r0 = *(u32*)((u8*)r4 + 0x2D0);
    if ((s32)r3 <= (s32)r0) goto L_800A23C8;
L_800A23E4: ;
    if ((u32)r5 != (u32)0x0) goto L_800A2418;
    r3 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r3 != (u32)0x0) goto L_800A2400;
    *(u32*)((u8*)r30 + 0x0) = r4;
    goto L_800A2404;
L_800A2400: ;
    *(u32*)((u8*)r3 + 0x2E0) = r4;
L_800A2404: ;
    *(u32*)((u8*)r4 + 0x2E4) = r3;
    r0 = 0x0;
    *(u32*)((u8*)r4 + 0x2E0) = r0;
    *(u32*)((u8*)r30 + 0x4) = r4;
    goto L_800A243C;
L_800A2418: ;
    *(u32*)((u8*)r4 + 0x2E0) = r5;
    r3 = *(u32*)((u8*)r5 + 0x2E4);
    *(u32*)((u8*)r5 + 0x2E4) = r4;
    *(u32*)((u8*)r4 + 0x2E4) = r3;
    if ((u32)r3 != (u32)0x0) goto L_800A2438;
    *(u32*)((u8*)r30 + 0x0) = r4;
    goto L_800A243C;
L_800A2438: ;
    *(u32*)((u8*)r3 + 0x2E0) = r4;
L_800A243C: ;
    r0 = 0x1;
    *(u32*)RunQueueHint_8047A764 = r0;
    r0 = *(u32*)RunQueueHint_8047A764;
    if ((s32)r0 == (s32)0x0) goto L_800A2458;
    r3 = 0x0;
    SelectThread();
L_800A2458: ;
    r3 = r31;
    OSRestoreInterrupts();
    return;
}
#pragma pop

/* fn_800A2478 - 0x800A2478 | size: 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A2478(void) {
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    OSDisableInterrupts();
    r4 = (u32)RunQueue_803FB898;
    r31 = r3 + 0x0;
    r5 = (u32)RunQueue_803FB898;
    goto L_800A253C;
L_800A24A4: ;
    r3 = *(u32*)((u8*)r6 + 0x2E0);
    if ((u32)r3 != (u32)0x0) goto L_800A24BC;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x4) = r0;
    goto L_800A24C4;
L_800A24BC: ;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x2E4) = r0;
L_800A24C4: ;
    *(u32*)((u8*)r30 + 0x0) = r3;
    r0 = 0x1;
    *(u16*)((u8*)r6 + 0x2C8) = r0;
    r0 = *(u32*)((u8*)r6 + 0x2CC);
    if ((s32)r0 > (s32)0x0) goto L_800A253C;
    r0 = *(u32*)((u8*)r6 + 0x2D0);
    r0 = r0 << 3;
    r0 = r5 + r0;
    *(u32*)((u8*)r6 + 0x2DC) = r0;
    r4 = *(u32*)((u8*)r6 + 0x2DC);
    r3 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r3 != (u32)0x0) goto L_800A2504;
    *(u32*)((u8*)r4 + 0x0) = r6;
    goto L_800A2508;
L_800A2504: ;
    *(u32*)((u8*)r3 + 0x2E0) = r6;
L_800A2508: ;
    *(u32*)((u8*)r6 + 0x2E4) = r3;
    r0 = 0x0;
    r3 = 0x1;
    *(u32*)((u8*)r6 + 0x2E0) = r0;
    r4 = *(u32*)((u8*)r6 + 0x2DC);
    *(u32*)((u8*)r4 + 0x4) = r6;
    r0 = *(u32*)((u8*)r6 + 0x2D0);
    r4 = *(u32*)RunQueueBits_8047A760;
    r0 = 0x1f - r0;
    r0 = r3 << r0;
    r0 = r4 | r0;
    *(u32*)RunQueueBits_8047A760 = r0;
    *(u32*)RunQueueHint_8047A764 = r3;
L_800A253C: ;
    r6 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r6 != (u32)0x0) goto L_800A24A4;
    r0 = *(u32*)RunQueueHint_8047A764;
    if ((s32)r0 == (s32)0x0) goto L_800A255C;
    r3 = 0x0;
    SelectThread();
L_800A255C: ;
    r3 = r31;
    OSRestoreInterrupts();
    return;
}
#pragma pop

/* fn_800A257C - 0x800A257C | size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A257C(void) {
    extern void fn_800A14EC();
    extern void fn_800A1528();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r31, r4 */;
    r29 = r3 + 0x0;
    if ((s32)r0 < (s32)0) goto L_800A25A8;
    if ((s32)r31 <= (s32)0x1f) goto L_800A25B0;
L_800A25A8: ;
    r3 = 0x0;
    goto L_800A2620;
L_800A25B0: ;
    OSDisableInterrupts();
    r0 = *(u32*)((u8*)r29 + 0x2D4);
    r30 = r3 + 0x0;
    if ((s32)r0 == (s32)r31) goto L_800A2614;
    *(u32*)((u8*)r29 + 0x2D4) = r31;
    r31 = r29;
L_800A25CC: ;
    r0 = *(u32*)((u8*)r31 + 0x2CC);
    if ((s32)r0 > (s32)0x0) goto L_800A2600;
    r3 = r31;
    fn_800A14EC();
    r0 = *(u32*)((u8*)r31 + 0x2D0);
    r4 = r3 + 0x0;
    if ((s32)r0 == (s32)r4) goto L_800A2600;
    r3 = r31;
    fn_800A1528();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)r4) goto L_800A25CC;
L_800A2600: ;
    r0 = *(u32*)RunQueueHint_8047A764;
    if ((s32)r0 == (s32)0x0) goto L_800A2614;
    r3 = 0x0;
    SelectThread();
L_800A2614: ;
    r3 = r30;
    OSRestoreInterrupts();
    r3 = 0x1;
L_800A2620: ;
    return;
}
#pragma pop

/* fn_800A263C - 0x800A263C | size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A263C(void) {
    extern void fn_800A19CC();
    extern void fn_800A1F94();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r31 = 0;

    r8 = (u32)RunQueue_803FB898;
    r10 = r4 + 0x0;
    r9 = r5 + 0x0;
    r7 = r6 + 0x0;
    r31 = (u32)RunQueue_803FB898;
    if ((u32)r3 == (u32)0x0) goto L_800A26A0;
    r0 = *(u16*)((u8*)r31 + 0x3C8);
    if ((u32)r0 != (u32)0x0) goto L_800A26B4;
    r4 = r3 + 0x0;
    r6 = r9 + 0x0;
    r5 = r10 + 0x0;
    r3 = r31 + 0x100;
    r8 = 0x1f;
    r9 = 0x1;
    fn_800A19CC();
    r3 = r31 + 0x100;
    fn_800A1F94();
    r3 = r31 + 0x100;
    goto L_800A26B8;
L_800A26A0: ;
    r0 = *(u16*)((u8*)r31 + 0x3C8);
    if ((u32)r0 == (u32)0x0) goto L_800A26B4;
    r3 = r31 + 0x100;
    OSCancelThread();
L_800A26B4: ;
    r3 = 0x0;
L_800A26B8: ;
    return;
}
#pragma pop

