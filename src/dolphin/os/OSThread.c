#include "dolphin/os/OSThread.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSTime.h"

/*
 * OSThread.c - Thread management.
 *
 * Implements thread initialization, scheduling, and thread queue management.
 *
 * Matches: 0x800A1290 - 0x800A2774
 */

/* Thread state values */
#define OS_THREAD_STATE_READY    1
#define OS_THREAD_STATE_RUNNING  2
#define OS_THREAD_STATE_WAITING  4
#define OS_THREAD_STATE_MORIBUND 8

/* Memory-mapped thread pointers */
#define OS_CURRENT_THREAD       (*(OSThread* volatile*)0x800000E4)
#define OS_CURRENT_FPU_CONTEXT  (*(OSContext* volatile*)0x800000D8)
#define OS_THREAD_LINK_HEAD     (*(OSThread* volatile*)0x800000DC)
#define OS_THREAD_LINK_TAIL     (*(OSThread* volatile*)0x800000E0)

typedef struct RunQueueEntry {
    OSThread* head;
    OSThread* tail;
} RunQueueEntry;

/* The run queue is an array of 32 priority levels */
typedef struct RunQueueData {
    RunQueueEntry queue[32]; /* 0x000 - 0x0FF: priority run queues */
    u8  pad1[0x418 - 0x100];
    OSThread defaultThread;  /* 0x418 */
    u16 state;               /* 0x6E0 */
    u16 attr;                /* 0x6E2 */
    u32 suspend;             /* 0x6E4 */
    s32 maxPriority;         /* 0x6E8 */
    s32 defaultPriority;     /* 0x6EC */
    s32 basePriority;        /* 0x6F0 */
    u8  pad2[0x708 - 0x6F4];
    void* mutex;             /* 0x708 */
    void* mutexQueue;        /* 0x70C */
    void* mutexQueueTail;    /* 0x710 */
    u8  pad3[0x71C - 0x714];
    u32* stackBase;          /* 0x71C */
    u32* stackEnd;           /* 0x720 */
    u8  pad4[0x730 - 0x724];
    OSContext idleContext;    /* 0x730 */
} RunQueueData;

static RunQueueData RunQueue;
static u32  RunQueueBits;
static BOOL RunQueueHint;
static s32  Reschedule;

static OSSwitchThreadCallback SwitchThreadCallback;

extern u32  _stack_addr[];
extern u32  _stack_end[];

static void UnsetRun(OSThread* thread);
static void SelectThread(BOOL yield);

void __OSThreadInit(void) {
    OSThread* defaultThread;

    RunQueue.state          = 2;   /* Running */
    RunQueue.attr           = 1;   /* Detached */
    RunQueue.suspend        = 0;
    RunQueue.basePriority   = -1;
    RunQueue.maxPriority    = 16;
    RunQueue.defaultPriority = 16;
    RunQueue.mutex          = NULL;

    defaultThread = &RunQueue.defaultThread;

    OSInitThreadQueue((OSThreadQueue*)(((u8*)&RunQueue) + 0x418 + 0x2E8));

    RunQueue.mutexQueueTail = NULL;
    RunQueue.mutexQueue     = NULL;

    OS_CURRENT_FPU_CONTEXT = (OSContext*)defaultThread;
    OSClearContext(&defaultThread->context);
    OSSetCurrentContext(&defaultThread->context);

    RunQueue.stackBase = (u32*)_stack_addr;
    RunQueue.stackEnd  = (u32*)_stack_end;
    *(RunQueue.stackEnd) = 0xDEADBABE;

    /* Call the switch thread callback */
    {
        OSThread* oldThread;
        oldThread = OS_CURRENT_THREAD;
        SwitchThreadCallback(oldThread, defaultThread);
    }

    OS_CURRENT_THREAD = defaultThread;

    OSClearStack(0);

    RunQueueBits = 0;
    RunQueueHint = FALSE;

    {
        s32 i;
        for (i = 0; i <= 31; i++) {
            OSInitThreadQueue((OSThreadQueue*)&RunQueue.queue[i]);
        }
    }

    /* Init the global thread list */
    {
        OSThreadQueue* activeQueue = (OSThreadQueue*)0x800000DC;
        OSInitThreadQueue(activeQueue);

        /* Link default thread into the active list */
        {
            OSThread* tail;
            tail = *(OSThread**)(0x800000E0);
            if (tail == NULL) {
                *(OSThread**)0x800000DC = defaultThread;
            } else {
                tail->linkActive.next = defaultThread;
            }
            defaultThread->linkActive.prev = tail;
            defaultThread->linkActive.next = NULL;
            *(OSThread**)(0x800000E0) = defaultThread;
        }
    }

    /* Clear the idle context */
    OSClearContext((OSContext*)(((u8*)&RunQueue) + 0x730));

    Reschedule = 0;
}

void OSInitThreadQueue(OSThreadQueue* queue) {
    queue->tail = NULL;
    queue->head = NULL;
}

s32 OSDisableScheduler(void) {
    BOOL enabled;
    s32 old;

    enabled = OSDisableInterrupts();
    old = Reschedule;
    Reschedule = old + 1;
    OSRestoreInterrupts(enabled);
    return old;
}

s32 OSEnableScheduler(void) {
    BOOL enabled;
    s32 old;

    enabled = OSDisableInterrupts();
    old = Reschedule;
    Reschedule = old - 1;
    OSRestoreInterrupts(enabled);
    return old;
}

static void SelectThread(BOOL yield) {
    OSContext* currentCtx;
    OSThread*  currentThread;
    OSThread*  nextThread;
    s32        priority;
    u32        bits;

    currentCtx    = (OSContext*)OS_CURRENT_THREAD;
    currentThread = OS_CURRENT_THREAD;

    if (Reschedule > 0) {
        return;
    }

    OSSetCurrentContext(currentCtx);

    if (currentThread != NULL) {
        if (currentThread->state == OS_THREAD_STATE_RUNNING) {
            if (!yield) {
                /* Find the highest priority ready thread */
                bits = RunQueueBits;
                if (bits == 0) {
                    return;
                }

                /* Count leading zeros to find the highest priority */
                {
                    u32 tmp = bits;
                    priority = 0;
                    if (!(tmp & 0xFFFF0000)) { priority += 16; tmp <<= 16; }
                    if (!(tmp & 0xFF000000)) { priority += 8;  tmp <<= 8;  }
                    if (!(tmp & 0xF0000000)) { priority += 4;  tmp <<= 4;  }
                    if (!(tmp & 0xC0000000)) { priority += 2;  tmp <<= 2;  }
                    if (!(tmp & 0x80000000)) { priority += 1; }
                }

                if (currentThread->priority <= priority) {
                    return;
                }
            }
        }
    }

    /* Find the best thread to run */
    bits = RunQueueBits;
    if (bits == 0) {
        /* No threads ready - idle */
        OSSetCurrentContext((OSContext*)(((u8*)&RunQueue) + 0x730));
        do {
            OSEnableInterrupts();
            bits = RunQueueBits;
            OSDisableInterrupts();
        } while (bits == 0);
        OSSetCurrentContext(currentCtx);
    }

    /* Find highest priority */
    {
        u32 tmp = bits;
        priority = 0;
        if (!(tmp & 0xFFFF0000)) { priority += 16; tmp <<= 16; }
        if (!(tmp & 0xFF000000)) { priority += 8;  tmp <<= 8;  }
        if (!(tmp & 0xF0000000)) { priority += 4;  tmp <<= 4;  }
        if (!(tmp & 0xC0000000)) { priority += 2;  tmp <<= 2;  }
        if (!(tmp & 0x80000000)) { priority += 1; }
    }

    /* Pop thread from the run queue */
    nextThread = RunQueue.queue[priority].head;
    /* Dequeue from run queue */
    RunQueue.queue[priority].head = nextThread->link.next;
    if (nextThread->link.next == NULL) {
        RunQueue.queue[priority].tail = NULL;
        RunQueueBits &= ~(1 << (31 - priority));
    } else {
        nextThread->link.next->link.prev = NULL;
    }
    nextThread->queue = NULL;

    if (nextThread == currentThread) {
        nextThread->state = OS_THREAD_STATE_RUNNING;
        return;
    }

    if (currentThread != NULL && currentThread->state == OS_THREAD_STATE_RUNNING) {
        currentThread->state = OS_THREAD_STATE_READY;
        /* Enqueue current thread into its run queue */
        {
            s32 prio = currentThread->priority;
            currentThread->link.prev = RunQueue.queue[prio].tail;
            currentThread->link.next = NULL;
            if (RunQueue.queue[prio].tail != NULL) {
                RunQueue.queue[prio].tail->link.next = currentThread;
            } else {
                RunQueue.queue[prio].head = currentThread;
            }
            RunQueue.queue[prio].tail = currentThread;
            currentThread->queue = (OSThreadQueue*)&RunQueue.queue[prio];
            RunQueueBits |= (1 << (31 - prio));
        }
    }

    RunQueueHint = FALSE;
    nextThread->state = OS_THREAD_STATE_RUNNING;
    OS_CURRENT_THREAD = nextThread;

    OSSetCurrentContext(&nextThread->context);
    OSLoadContext(&nextThread->context);
}

void __OSReschedule(void) {
    if (RunQueueHint == FALSE) {
        return;
    }
    SelectThread(FALSE);
}

void OSCancelThread(OSThread* thread) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    switch (thread->state) {
    case OS_THREAD_STATE_READY:
        /* Remove from run queue */
        if (thread->queue != NULL) {
            /* Unlink from thread queue */
            if (thread->link.prev != NULL) {
                thread->link.prev->link.next = thread->link.next;
            } else {
                thread->queue->head = thread->link.next;
            }
            if (thread->link.next != NULL) {
                thread->link.next->link.prev = thread->link.prev;
            } else {
                thread->queue->tail = thread->link.prev;
            }

            /* Clear bits if queue empty */
            if (thread->queue->head == NULL) {
                RunQueueBits &= ~(1 << (31 - thread->priority));
            }
            thread->queue = NULL;
        }
        break;

    case OS_THREAD_STATE_RUNNING:
        break;

    case OS_THREAD_STATE_WAITING:
        /* Remove from wait queue */
        if (thread->queue != NULL) {
            if (thread->link.prev != NULL) {
                thread->link.prev->link.next = thread->link.next;
            } else {
                thread->queue->head = thread->link.next;
            }
            if (thread->link.next != NULL) {
                thread->link.next->link.prev = thread->link.prev;
            } else {
                thread->queue->tail = thread->link.prev;
            }
            thread->queue = NULL;
        }
        break;
    }

    thread->state = OS_THREAD_STATE_MORIBUND;
    __OSReschedule();

    OSRestoreInterrupts(enabled);
}

void OSClearStack(u8 val) {
    u32 sp;
    u32* stackEnd;
    u32  pattern;
    u32* p;
    u32  count;

    /* Build the fill pattern from val repeated 4 times */
    pattern = (val << 24) | (val << 16) | (val << 8) | val;

    /* Get current stack pointer (approximately) */
    sp = (u32)&sp;

    /* Get the stack end for the current thread */
    stackEnd = (u32*)(*(u32*)(0x800000E4 + 0x308));
    stackEnd += 1;

    if (stackEnd >= (u32*)sp) {
        return;
    }

    /* Fill stack memory with pattern */
    count = (((u32)sp - (u32)stackEnd + 3) / 4);
    p = stackEnd;

    {
        u32 blocks = count >> 3;
        u32 ctr;

        if (blocks > 0) {
            for (ctr = 0; ctr < blocks; ctr++) {
                p[0] = pattern;
                p[1] = pattern;
                p[2] = pattern;
                p[3] = pattern;
                p[4] = pattern;
                p[5] = pattern;
                p[6] = pattern;
                p[7] = pattern;
                p += 8;
            }
        }

        count &= 7;
        while (count-- > 0) {
            *p++ = pattern;
        }
    }
}
