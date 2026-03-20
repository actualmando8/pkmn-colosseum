#include "dolphin/types.h"

/*
 * TRKTarget.c - TRK target control functions.
 *
 * Manages the stopped/running state of the target program,
 * handles interrupt events, and processes support requests
 * (system calls from the target via trap instructions).
 */

extern void MWTRACE(s32 level, const char* fmt, ...);
extern void fn_800BE464(void* event, s32 type);
extern s32  fn_800BE47C(void* event);
extern s32  fn_800C0CD8(s32 event);
extern s32  fn_800C07A4(u32 addr, u8 type, u32 count, void* result);
extern s32  fn_800C06BC(u32 addr, void* result);
extern s32  fn_800C05AC(u32 addr, void* data, u8 type, void* result, void* length);
extern s32  fn_800C0AA0(u32 addr, u32 count, void* buf, void* result, u32 flags, u32 isD1, u32 step);
extern void fn_800C0D70(u32 addr, u32 size);
extern s32  fn_800C25B0(u8* buf, u32 pc);

extern void TRKTargetSetStopped(s32 stopped);
extern void UnreserveEXI2Port(void);
extern void ReserveEXI2Port(void);
extern void TRKSwapAndGo(void);

/* TRK state and CPU state structures */
extern u8 gTRKState[];     /* large state structure */
extern u8 gTRKCPUState[];  /* saved CPU context */

/* Breakpoint info at lbl_80313834 */
extern u8 lbl_80313834[];

/*
 * TRKTargetSetInputPendingPtr - Store the input pending flag pointer.
 * This pointer is checked by interrupt handlers to determine if
 * there is pending serial input.
 */
void TRKTargetSetInputPendingPtr(u8* ptr) {
    /* gTRKState offset 0xA0 = inputPendingPtr */
    *(u32*)&gTRKState[0xA0] = (u32)ptr;
}

/*
 * TRKTargetSetStopped - Set the target stopped flag.
 * 0 = running, 1 = stopped.
 */
void TRKTargetSetStopped(s32 stopped) {
    /* gTRKState offset 0x98 = stopped flag */
    *(s32*)&gTRKState[0x98] = stopped;
}

/*
 * TRKTargetStopped - Query whether the target is stopped.
 * Returns 1 if stopped, 0 if running.
 */
s32 TRKTargetStopped(void) {
    return *(s32*)&gTRKState[0x98];
}

/*
 * TRKTargetSupportRequest - Handle a system call from the target.
 *
 * The target communicates with the debugger by executing special
 * trap instructions. The "reason" code in GPR3 (offset 0x0C in
 * the saved CPU state) determines what operation is requested:
 *   0xD0, 0xD1: Memory read/write with step
 *   0xD2: Read from address
 *   0xD3: Open file
 *   0xD4: Write to file
 */
void TRKTargetSupportRequest(void) {
    u8* cpuState = gTRKCPUState;
    s32 reason;
    s32 result;
    s32 err;

    reason = *(s32*)&cpuState[0x0C]; /* GPR3 = reason code */

    /* If not a recognized support request, post a "stop" event */
    if (reason != 0xD1 && reason != 0xD0 && reason != 0xD2 &&
        reason != 0xD3 && reason != 0xD4) {
        u8 eventBuf[0x10];
        fn_800BE464((void*)eventBuf, 4);
        fn_800BE47C((void*)eventBuf);
        return;
    }

    if (reason == 0xD2) {
        /* Read memory request */
        u32 addr = *(u32*)&cpuState[0x10]; /* GPR4 = address */
        u8  type = (u8)(*(u32*)&cpuState[0x14]); /* GPR5 = type */
        u32 count = *(u32*)&cpuState[0x18]; /* GPR6 = count */
        u32 resultCode = 0;

        err = fn_800C07A4(addr, type, count, &resultCode);

        if (resultCode == 0 && err != 0) {
            resultCode = 1;
        }
        *(u32*)&cpuState[0x0C] = resultCode;
    } else if (reason == 0xD3) {
        /* File open request */
        u32 addr = *(u32*)&cpuState[0x10]; /* GPR4 = filename addr */
        u32 resultCode = 0;

        err = fn_800C06BC(addr, &resultCode);

        if (resultCode == 0 && err != 0) {
            resultCode = 1;
        }
        *(u32*)&cpuState[0x0C] = resultCode;
    } else if (reason == 0xD4) {
        /* File write request */
        u32 ptrAddr = *(u32*)&cpuState[0x14]; /* GPR5 = data pointer addr */
        u32 count = *(u32*)&cpuState[0x18]; /* GPR6 = count */
        u32 data = *(u32*)ptrAddr;
        u32 addr = *(u32*)&cpuState[0x10]; /* GPR4 = file handle */
        u8  type = (u8)count;
        u32 resultCode = 0;

        err = fn_800C05AC(addr, &data, type, &resultCode, &data);

        if (resultCode == 0 && err != 0) {
            resultCode = 1;
        }
        *(u32*)&cpuState[0x0C] = resultCode;
        *(u32*)ptrAddr = data;
    } else {
        /* 0xD0 or 0xD1: generic memory access */
        u32 addr = *(u32*)&cpuState[0x10]; /* GPR4 */
        u32 count = *(u32*)&cpuState[0x18]; /* GPR6 */
        u32* buf = (u32*)&cpuState[0x14]; /* GPR5 = buffer */
        u32 isD1 = (reason == 0xD1) ? 1 : 0;
        u32 resultCode = 0;

        err = fn_800C0AA0(addr, count, (void*)buf, &resultCode, (u32)*buf, isD1, 1);

        if (resultCode == 0 && err != 0) {
            resultCode = 1;
        }
        *(u32*)&cpuState[0x0C] = resultCode;

        if (reason == 0xD1) {
            fn_800C0D70(count, *(u32*)buf);
        }
    }

    /* Advance PC past the trap instruction */
    {
        u32 pc = *(u32*)&cpuState[0x80]; /* SRR0/PC */
        *(u32*)&cpuState[0x80] = pc + 4;
    }
}

/*
 * TRKTargetInterrupt - Handle a debugger interrupt event.
 *
 * Checks if the interrupt should cause a stop based on breakpoint
 * configuration. If a breakpoint match is found, sets the target
 * as stopped and signals a break event.
 */
s32 TRKTargetInterrupt(void* event) {
    s32 result = 0;
    s32 eventType = *(s32*)event;

    /* Only handle types 3 and 4 */
    if (eventType < 3 || eventType >= 5) {
        goto done;
    }

    {
        u8* bpInfo = lbl_80313834;
        s32 bpActive = *(s32*)&bpInfo[0];

        if (bpActive == 0) {
            goto check_stop;
        }

        {
            u8* cpuState = gTRKCPUState;
            s32 doStop = 1;
            u32 msr;

            /* Clear single-step bit in MSR (bit 10) */
            msr = *(u32*)&cpuState[0x1F8];
            msr &= ~0x400;
            *(u32*)&cpuState[0x1F8] = msr;

            if (doStop == 0) {
                goto check_stop_2;
            }

            /* Check if this is a trace exception (0xD00) */
            {
                u32 excID = *(u32*)&cpuState[0x2F8] & 0xFFFF;
                if (excID == 0xD00) {
                    s32 bpType = *(s32*)&bpInfo[4];

                    if (bpType == 1) {
                        /* Range breakpoint: check if PC is in range */
                        u32 pc = *(u32*)&cpuState[0x80];
                        u32 rangeStart = *(u32*)&bpInfo[0x0C];
                        u32 rangeEnd = *(u32*)&bpInfo[0x10];

                        if (pc >= rangeStart && pc <= rangeEnd) {
                            doStop = 0;
                        }
                    } else if (bpType == 0) {
                        /* Count breakpoint */
                        u32 count = *(u32*)&bpInfo[8];
                        if (count != 0) {
                            doStop = 0;
                        }
                    }
                }
            }

            if (doStop != 0) {
                /* Clear breakpoint and go to check_stop */
                *(s32*)&bpInfo[0] = 0;
                goto check_stop;
            }

        check_stop_2:
            /* Breakpoint still active */
            *(s32*)&bpInfo[0] = 1;

            MWTRACE(1, "TRKTargetInterrupt: stepping\n");

            /* Re-enable single-step bit */
            msr = *(u32*)&cpuState[0x1F8];
            msr |= 0x400;
            *(u32*)&cpuState[0x1F8] = msr;

            {
                s32 bpType2 = *(s32*)&bpInfo[4];
                if (bpType2 == 0 || bpType2 == 0x10) {
                    /* Decrement step count */
                    u32 count2 = *(u32*)&bpInfo[8];
                    *(u32*)&bpInfo[8] = count2 - 1;
                }
            }

            /* Clear stopped flag */
            *(s32*)&gTRKState[0x98] = 0;
        }
    }

check_stop:
    {
        u8* bpInfo2 = lbl_80313834;
        s32 bpActive2 = *(s32*)&bpInfo2[0];

        if (bpActive2 != 0) {
            goto done;
        }

        /* Set stopped and signal break event */
        *(s32*)&gTRKState[0x98] = 1;
        result = fn_800C0CD8(0x90);
    }

done:
    return result;
}

/*
 * TRKPostInterruptEvent - Determine event type and post to queue.
 *
 * Examines the saved exception ID to determine what kind of event
 * to post: support request (0xD00 trap), break, or generic interrupt.
 * Called from interrupt handler assembly code.
 */
void TRKPostInterruptEvent(void) {
    u8* state = gTRKState;

    /* Check inputNotify flag at offset 0x9C */
    if (*(s32*)&state[0x9C] != 0) {
        *(s32*)&state[0x9C] = 0;
        return;
    }

    {
        u32 excID;
        s32 eventType;
        u8 eventBuf[0x10];
        u8 readBuf[8];

        excID = *(u32*)&gTRKCPUState[0x2F8] & 0xFFFF;

        if (excID == 0xD00 || excID == 0x700) {
            /* Check if this is a support request (trap) */
            u32 pc = *(u32*)&gTRKCPUState[0x80]; /* SRR0 */

            fn_800C25B0(readBuf, pc);

            {
                u32 instr = *(u32*)&readBuf[0];
                /* Check for tw 31,0,0 (0x0FE00000) = support request trap */
                if (instr == 0x0FE00000) {
                    eventType = 5; /* support request */
                } else {
                    eventType = 3; /* break */
                }
            }
        } else {
            eventType = 4; /* generic interrupt */
        }

        fn_800BE464((void*)eventBuf, eventType);
        fn_800BE47C((void*)eventBuf);
    }
}

/*
 * TRKTargetContinue - Resume target execution.
 *
 * Clears the stopped flag, unreserves the EXI port so the target
 * can use it, swaps to target context, then re-reserves the port
 * when the target stops again.
 */
s32 TRKTargetContinue(void) {
    TRKTargetSetStopped(0);
    UnreserveEXI2Port();
    TRKSwapAndGo();
    ReserveEXI2Port();
    return 0;
}
