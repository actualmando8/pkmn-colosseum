#include "dolphin/types.h"

/*
 * TRKNub.c - MetroTRK debugger nub main loop and event queue.
 *
 * Implements the core event loop that processes debugger commands,
 * manages the event queue, and coordinates with target continue/stop.
 */

/* Forward declarations for internal TRK functions */
extern void* gTRKInputPendingPtr;
extern void fn_800C0CC0(void* mutex);  /* TRKReleaseMutex */
extern void fn_800C0CC8(void* mutex);  /* TRKAcquireMutex */
extern void fn_800C0CD0(void* mutex);  /* TRKInitializeMutex */
extern void fn_800BEEB4(s32 idx);      /* TRKReleaseBuffer */
extern void fn_800BE464(void* event, s32 type); /* TRKConstructEvent */
extern s32  fn_800BE47C(void* event);  /* TRKPostEvent */
extern void fn_80003488(void* dst, const void* src, u32 size);
extern void fn_800BF080(void);         /* TRKTerminateSerialHandler */
extern s32  fn_800BF1FC(void);         /* TRKProcessInput */
extern void fn_800BEE44(void);         /* TRKResetBuffer */

extern void MWTRACE(s32 level, const char* fmt, ...);
extern void usr_put_initialize(void);

/* Forward declarations for functions defined later in this file */
void TRKDestructEvent(void* event);
s32 TRKGetNextEvent(void* event);

/* Functions in this translation unit or nearby */
extern void* TRKGetBuffer(s32 index);
extern s32 TRKDispatchMessage(void* buffer);
extern void TRKTargetInterrupt(void* event);
extern void TRKTargetSupportRequest(void);
extern s32 TRKTargetStopped(void);
extern s32 TRKTargetContinue(void);
extern void TRKGetInput(void);
extern void TRKTargetSetInputPendingPtr(u8* ptr);
extern s32 TRKInitializeMessageBuffers(void);
extern s32 TRKInitializeDispatcher(void);
extern void InitializeProgramEndTrap(void);
extern s32 TRKInitializeSerialHandler(void);
extern s32 TRKInitializeTarget(void);
extern s32 TRKInitializeIntDrivenUART(s32 baud, s32 polarity, s32 pad, void* pendingPtr);
extern void TRK_board_display(const char* msg);

/* Event queue structure - 0x28 bytes at lbl_803FCDD8 */
/* Offset 0x00: mutex (4 bytes)
 * Offset 0x04: count (4 bytes)
 * Offset 0x08: head  (4 bytes)
 * Offset 0x0C: events[0] (12 bytes each, 2 entries)
 * Offset 0x24: sequence counter (4 bytes)
 */
extern u8 lbl_803FCDD8[];

/*
 * TRKNubMainLoop - Main debugger event processing loop.
 *
 * Continually dequeues events and dispatches them based on type:
 *   type 1: shutdown
 *   type 2: message - dispatch to command handler
 *   type 3/4: interrupt - send to target interrupt handler
 *   type 5: support request
 *
 * When no events are pending, checks for input or continues target.
 */
void TRKNubMainLoop(void) {
    s32 done = 0;
    s32 firstPass = 0;
    u8 eventBuf[0x10]; /* stack event storage: type at +0, padding at +4, bufIdx at +8 */

    while (done == 0) {
        if (TRKGetNextEvent((void*)eventBuf)) {
            s32 eventType;
            firstPass = 0;

            eventType = *(s32*)&eventBuf[0];
            switch (eventType) {
                case 2: { /* message event */
                    s32 bufIdx = *(s32*)&eventBuf[8];
                    void* buf = TRKGetBuffer(bufIdx);
                    TRKDispatchMessage(buf);
                    break;
                }
                case 1: /* shutdown */
                    done = 1;
                    break;
                case 3: /* break */
                case 4: /* interrupt */
                    TRKTargetInterrupt((void*)eventBuf);
                    break;
                case 5: /* support request */
                    TRKTargetSupportRequest();
                    break;
                default:
                    break;
            }
            TRKDestructEvent((void*)eventBuf);
        } else {
            if (firstPass != 0) {
                u8** ppInput = (u8**)&gTRKInputPendingPtr;
                u8* pInput = *ppInput;
                if (*pInput != 0) {
                    /* Input pending flag was cleared, try to continue */
                    goto try_continue;
                }
            }
            firstPass = 1;
            TRKGetInput();
            continue;

        try_continue:
            if (!TRKTargetStopped()) {
                TRKTargetContinue();
            }
            firstPass = 0;
        }
    }
}

/*
 * TRKDestructEvent - Release resources associated with an event.
 * Frees the buffer referenced by the event's buffer index field.
 */
void TRKDestructEvent(void* event) {
    s32 bufIdx = ((s32*)event)[2]; /* offset 0x08 */
    fn_800BEEB4(bufIdx);
}

/*
 * TRKGetNextEvent - Dequeue the next event from the event queue.
 * Returns 1 if an event was dequeued, 0 if queue was empty.
 */
s32 TRKGetNextEvent(void* event) {
    s32 result = 0;
    void* queue = (void*)lbl_803FCDD8;

    fn_800C0CC8(queue); /* acquire mutex */

    {
        s32 count = ((s32*)queue)[1]; /* offset 0x04 */
        if (count > 0) {
            s32 head = ((s32*)queue)[2]; /* offset 0x08 */
            void* src = (void*)((u8*)queue + 0x0C + head * 0x0C);
            fn_80003488(event, src, 0x0C);

            head = head + 1;
            count = count - 1;
            ((s32*)queue)[2] = head;
            if (head == 2) {
                ((s32*)queue)[2] = 0;
            }
            ((s32*)queue)[1] = count;
            result = 1;
        }
    }

    fn_800C0CC0(queue); /* release mutex */
    return result;
}

/*
 * TRKInitializeEventQueue - Set up the event queue for use.
 * Creates and acquires the mutex, then zeros the queue state.
 */
s32 TRKInitializeEventQueue(void) {
    void* queue = (void*)lbl_803FCDD8;

    fn_800C0CD0(queue); /* init mutex */
    fn_800C0CC8(queue); /* acquire mutex */

    ((s32*)queue)[1] = 0;   /* count = 0 */
    ((s32*)queue)[2] = 0;   /* head = 0 */
    ((s32*)queue)[9] = 0x100; /* sequence = 0x100 */

    fn_800C0CC0(queue); /* release mutex */

    return 0;
}

/*
 * TRKNubWelcome - Display the TRK welcome banner.
 */
void TRKNubWelcome(void) {
    TRK_board_display("MetroTRK for Dolphin different from R4.0");
}

/*
 * TRKTerminateNub - Shut down the debugger nub.
 * Calls the serial handler terminator.
 */
s32 TRKTerminateNub(void) {
    fn_800BF080();
    return 0;
}

/*
 * TRKInitializeNub - Initialize all TRK subsystems.
 *
 * Performs endian detection, then initializes the event queue,
 * message buffers, dispatcher, serial handler, target, and UART.
 * Returns 0 on success, nonzero on failure.
 */
s32 TRKInitializeNub(void) {
    s32 err = 0;
    s32 uartErr;
    u8 testBytes[4];

    /* Endian detection pattern: 0x12345678 */
    testBytes[0] = 0x12;
    testBytes[1] = 0x34;
    testBytes[2] = 0x56;
    testBytes[3] = 0x78;

    {
        u32 testWord = *(u32*)testBytes;
        u32* pBigEndian;

        /* gTRKBigEndian at gTRKBigEndian address */
        extern u32 gTRKBigEndian;
        pBigEndian = &gTRKBigEndian;
        *pBigEndian = 1; /* assume big endian initially */

        if (testWord == 0x12345678) {
            *pBigEndian = 1; /* big endian confirmed */
        } else if (testWord == 0x78563412) {
            *pBigEndian = 0; /* little endian */
        } else {
            err = 1; /* unknown endianness */
        }
    }

    MWTRACE(1, "TRKInitializeNub - starting\n");

    if (err == 0) {
        usr_put_initialize();
    }

    if (err == 0) {
        err = TRKInitializeEventQueue();
    }

    if (err == 0) {
        err = TRKInitializeMessageBuffers();
    }

    if (err == 0) {
        err = TRKInitializeDispatcher();
    }

    InitializeProgramEndTrap();

    if (err == 0) {
        err = TRKInitializeSerialHandler();
    }

    if (err == 0) {
        err = TRKInitializeTarget();
    }

    if (err == 0) {
        u8** ppInput;
        u8* inputPtr;

        extern void* gTRKInputPendingPtr;
        ppInput = (u8**)&gTRKInputPendingPtr;

        uartErr = TRKInitializeIntDrivenUART(0xE100, 1, 0, (void*)ppInput);

        inputPtr = *ppInput;
        TRKTargetSetInputPendingPtr(inputPtr);

        if (uartErr != 0) {
            err = uartErr;
        }
    }

    return err;
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800BE800 - 0x800BE800 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BE800(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BE844 - 0x800BE844 | size: 0xF0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BE844(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BE934 - 0x800BE934 | size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BE934(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BE9CC - 0x800BE9CC | size: 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BE9CC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BEAB4 - 0x800BEAB4 | size: 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BEAB4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BEBB0 - 0x800BEBB0 | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BEBB0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BEC18 - 0x800BEC18 | size: 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BEC18(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BED14 - 0x800BED14 | size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BED14(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BEDA0 - 0x800BEDA0 | size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BEDA0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BEE74 - 0x800BEE74 | size: 0x40 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BEE74(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

