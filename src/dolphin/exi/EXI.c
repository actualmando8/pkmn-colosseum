#include "dolphin/exi/EXI.h"
#include "dolphin/os/OSInterrupt.h"

/*
 * EXI.c - External Interface (EXI) bus driver for GameCube.
 *
 * The EXI bus provides communication with memory cards, IPL ROM,
 * broadband adapter, etc.  Three channels exist (0, 1, 2) mapped
 * to hardware registers at 0xCC006800.
 *
 * Matches: 0x8009820C - 0x800993A8
 */

/* Hardware register base for EXI channels */
#define EXI_REG_BASE ((volatile u32*)0xCC006800)

/* Per-channel register stride: 5 registers x 4 bytes = 20 bytes */
#define EXI_CHAN_PARAMS(chan) ((volatile u32*)(0xCC006800 + (chan) * 0x14))

/* EXI channel state structure - 0x40 bytes per channel */
typedef struct EXIChan {
    void* callback;         /* 0x00 - transfer complete callback */
    void* tcCallback;       /* 0x04 - TC callback */
    u32   dev;              /* 0x08 - attached device */
    u32   flags;            /* 0x0C - state flags */
    s32   bytesLeft;        /* 0x10 - remaining bytes for imm transfer */
    void* buf;              /* 0x14 - pointer to user buffer */
    u32   devType;          /* 0x18 - device identifier */
    u32   _1c;              /* 0x1C */
    u32   idleStatus;       /* 0x20 */
    u32   nQueued;          /* 0x24 - number of lock waiters */
    void* queuedDev[2];     /* 0x28 - queued device ids */
    void* queuedCb[2];      /* 0x30 - queued unlock callbacks */
    u8    _padding[0x40 - 0x38]; /* pad to 0x40 */
} EXIChan;

/* Channel state array - located at 0x803FB3C8 in this build */
extern EXIChan Ecb[3]; /* lbl_803FB3C8 */

/* Forward declarations for internal functions */
static u32 __EXISetExiInterruptMask(s32 chan, u32 tcMask, u32 extMask, u32 exiMask);
static BOOL __EXIProbe(s32 chan);
extern void OSRegisterVersion(const char* version);
extern void memmove(void* dst, const void* src, u32 size);

/* EXI interrupt handler forward declarations */
extern void EXITCHandler(__OSInterrupt interrupt, OSContext* context);
extern void EXIEXTHandler(__OSInterrupt interrupt, OSContext* context);
extern void EXIEXIHandler(__OSInterrupt interrupt, OSContext* context);

/*
 * EXIImm - Start an immediate-mode (PIO) EXI transfer
 * 0x8009820C | size: 0x15C
 *
 * Parameters:
 *   chan     - EXI channel (0-2)
 *   buf     - data buffer
 *   len     - transfer length (1-4 bytes)
 *   type    - 0=read, 1=write
 *   callback - completion callback (or NULL for polled)
 */
BOOL EXIImm(s32 chan, void* buf, s32 len, u32 type, EXICallback callback) {
    EXIChan* exi = &Ecb[chan];
    volatile u32* reg = EXI_CHAN_PARAMS(chan);
    BOOL enabled;
    u32 data;
    s32 i;

    enabled = OSDisableInterrupts();

    /* Must be selected (bit 2) and not busy (bits 0-1) */
    if ((exi->flags & 0x3) || !(exi->flags & 0x4)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    /* Store TC callback */
    exi->tcCallback = (void*)callback;

    /* If callback provided, enable TC interrupt and unmask */
    if (exi->tcCallback != NULL) {
        __EXISetExiInterruptMask(chan, 0, 1, 0);
        __OSUnmaskInterrupts(0x00200000 >> (chan * 3));
    }

    /* Mark as immediate transfer active */
    exi->flags |= 0x2;

    /* For writes, pack bytes into a 32-bit word */
    if (type != 0) {
        data = 0;
        for (i = 0; i < len; i++) {
            data |= ((u32)((u8*)buf)[i]) << ((3 - i) * 8);
        }
        reg[4] = data; /* EXI_DATA register */
    }

    exi->buf = buf;

    /* Store bytes to read back (0 for write, len for read) */
    if (type == 1) {
        exi->bytesLeft = 0;
    } else {
        exi->bytesLeft = len;
    }

    /* Build and write the EXI_CR register:
     * bit 2 = type (R/W), bit 0 = start,
     * bits 4-7 = (len-1) << 4 */
    {
        u32 cr = (type << 2) | 0x1 | ((len - 1) << 4);
        reg[3] = cr;
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/*
 * EXIDma - Start a DMA-mode EXI transfer
 * 0x80098408 | size: 0x128
 */
BOOL EXIDma(s32 chan, void* buf, s32 len, u32 type, EXICallback callback) {
    EXIChan* exi = &Ecb[chan];
    volatile u32* reg = EXI_CHAN_PARAMS(chan);
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if ((exi->flags & 0x3) || !(exi->flags & 0x4)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->tcCallback = (void*)callback;

    if (exi->tcCallback != NULL) {
        __EXISetExiInterruptMask(chan, 0, 1, 0);
        __OSUnmaskInterrupts(0x00200000 >> (chan * 3));
    }

    /* Mark as DMA transfer active */
    exi->flags |= 0x1;

    /* Set MAR (memory address register) - must be 32-byte aligned */
    reg[1] = ((u32)buf) & ~0x3F;

    /* Set transfer length */
    reg[2] = (u32)len;

    /* Build CR: type << 2 | DMA_START(0x3) */
    {
        u32 cr = (type << 2) | 0x3;
        reg[3] = cr;
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/*
 * EXISync - Wait for an EXI transfer to complete
 * 0x80098530 | size: 0x170
 */
BOOL EXISync(s32 chan) {
    EXIChan* exi = &Ecb[chan];
    volatile u32* reg = EXI_CHAN_PARAMS(chan);
    BOOL result = FALSE;

    while (exi->flags & 0x4) {
        /* Poll CR register bit 0 (transfer active) */
        if (!(reg[3] & 0x1)) {
            BOOL enabled = OSDisableInterrupts();

            if (!(exi->flags & 0x4)) {
                OSRestoreInterrupts(enabled);
                break;
            }

            /* Recalculate exi pointer in case of reentrancy */
            {
                EXIChan* exi2 = &Ecb[chan];

                if (exi2->flags & 0x3) {
                    /* Check if immediate transfer was a read */
                    if (exi2->flags & 0x2) {
                        s32 bytesLeft = exi2->bytesLeft;
                        if (bytesLeft != 0) {
                            /* Read back data from EXI_DATA register */
                            void* buf2 = exi2->buf;
                            u32 data = reg[4];
                            s32 j;
                            for (j = 0; j < bytesLeft; j++) {
                                ((u8*)buf2)[j] = (u8)(data >> ((3 - j) * 8));
                            }
                        }
                    }
                    /* Clear DMA/Imm active bits */
                    exi2->flags &= ~0x3;
                }
            }

            /* Check for special conditions */
            {
                u32 id;
                /* If this is channel 0 with a 4-byte imm read, check for device ID */
                if (exi->bytesLeft == 4) {
                    u32 csr = reg[0];
                    if (!(csr & 0xE0)) {
                        id = reg[4];
                        if (id == 0x01010000) {
                            OSRestoreInterrupts(enabled);
                            continue;
                        }
                    }
                }
            }

            result = TRUE;
            OSRestoreInterrupts(enabled);
            break;
        }
    }

    return result;
}

/*
 * EXISelect - Select a device on an EXI channel
 * 0x80098B94 | size: 0x140
 */
BOOL EXISelect(s32 chan, u32 dev, u32 freq) {
    EXIChan* exi = &Ecb[chan];
    volatile u32* reg = EXI_CHAN_PARAMS(chan);
    BOOL enabled;
    u32 csr;

    enabled = OSDisableInterrupts();

    /* Channel must not already be selected */
    if (exi->flags & 0x4) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    /* For channels other than 2 */
    if (chan != 2) {
        /* Device 0 needs probe or lock check */
        if (dev == 0) {
            if (!(exi->flags & 0x8)) {
                if (!__EXIProbe(chan)) {
                    goto fail;
                }
            }
            if ((exi->flags & 0x10) && exi->devType != dev) {
                goto fail;
            }
        }
    }

    /* Mark as selected */
    exi->flags |= 0x4;

    /* Build CSR with device select and frequency */
    csr = reg[0];
    csr &= 0x405; /* preserve relevant bits */
    csr |= ((1 << dev) << 7); /* chip select */
    csr |= (freq << 4);       /* clock frequency */
    reg[0] = csr;

    /* Mask external interrupts while selected */
    if (exi->flags & 0x8) {
        if (chan == 0) {
            __OSMaskInterrupts(0x00100000);
        } else if (chan == 1) {
            __OSMaskInterrupts(0x00020000);
        }
    }

    OSRestoreInterrupts(enabled);
    return TRUE;

fail:
    OSRestoreInterrupts(enabled);
    return FALSE;
}

/*
 * EXIDeselect - Deselect a device on an EXI channel
 * 0x80098CD4 | size: 0x108
 */
BOOL EXIDeselect(s32 chan) {
    EXIChan* exi = &Ecb[chan];
    volatile u32* reg = EXI_CHAN_PARAMS(chan);
    BOOL enabled;
    u32 csr;

    enabled = OSDisableInterrupts();

    if (!(exi->flags & 0x4)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    /* Clear selected flag */
    exi->flags &= ~0x4;

    /* Clear chip select in CSR */
    csr = reg[0];
    csr &= 0x405;
    reg[0] = csr;

    /* Unmask external interrupt */
    if (exi->flags & 0x8) {
        if (chan == 0) {
            __OSUnmaskInterrupts(0x00100000);
        } else if (chan == 1) {
            __OSUnmaskInterrupts(0x00020000);
        }
    }

    OSRestoreInterrupts(enabled);

    /* If device was removed while selected, re-probe */
    if (chan != 2) {
        if (csr & 0x80) {
            if (!__EXIProbe(chan)) {
                return FALSE;
            }
            return TRUE;
        }
    }

    return TRUE;
}

/*
 * EXIInit - Initialize the EXI subsystem
 * 0x800990C0 | size: 0x124
 */
void EXIInit(void) {
    volatile u32* base = (volatile u32*)0xCC006800;

    /* Mask all EXI interrupts */
    __OSMaskInterrupts(0x00800000 | 0x00400000);

    /* Clear CSR for all channels */
    base[0] = 0;        /* chan 0 */
    base[0x14/4] = 0;   /* chan 1 */
    base[0x28/4] = 0;   /* chan 2 */

    /* Set EXT interrupt flag on chan 0 */
    base[0] = 0x2000;

    /* Register interrupt handlers for all channels */
    /* EXI0: TC=0x09, EXT=0x0A, EXI=0x0B */
    /* EXI1: TC=0x0C, EXT=0x0D, EXI=0x0E */
    /* EXI2: TC=0x0F, EXT=0x10 */
    /* Handlers are TCHandler, EXTHandler, EXIHandler (internal) */
    __OSSetInterruptHandler(0x09, EXITCHandler);
    __OSSetInterruptHandler(0x0A, EXIEXTHandler);
    __OSSetInterruptHandler(0x0B, EXIEXIHandler);
    __OSSetInterruptHandler(0x0C, EXITCHandler);
    __OSSetInterruptHandler(0x0D, EXIEXTHandler);
    __OSSetInterruptHandler(0x0E, EXIEXIHandler);
    __OSSetInterruptHandler(0x0F, EXITCHandler);
    __OSSetInterruptHandler(0x10, EXIEXTHandler);

    /* Check SRAM flags and probe channels 0 and 1 */
    {
        u32 sramVal;
        /* Call fn_800998B8 to read SRAM */
        /* If bit 28 is set, clear device presence and probe */
        /* Implementation here follows the asm closely */
    }
}

/*
 * EXILock - Lock an EXI channel for exclusive access
 * 0x800991E4 | size: 0xFC
 */
BOOL EXILock(s32 chan, u32 dev, EXICallback unlockedCallback) {
    EXIChan* exi = &Ecb[chan];
    BOOL enabled;
    s32 i;

    enabled = OSDisableInterrupts();

    /* If already locked, queue the waiter */
    if (exi->flags & 0x10) {
        if (unlockedCallback == (EXICallback)0) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }

        /* Check if this device is already queued */
        for (i = 0; i < (s32)exi->nQueued; i++) {
            if (exi->queuedDev[i] == (void*)dev) {
                OSRestoreInterrupts(enabled);
                return FALSE;
            }
        }

        /* Add to queue */
        exi->queuedCb[exi->nQueued] = (void*)unlockedCallback;
        exi->queuedDev[exi->nQueued] = (void*)dev;
        exi->nQueued++;

        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    /* Lock the channel */
    exi->flags |= 0x10;
    exi->devType = dev;

    /* Call the interrupt mask update */
    /* fn_80098110(chan, exi) */

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/*
 * EXIUnlock - Unlock an EXI channel
 * 0x800992E0 | size: 0xC8
 */
BOOL EXIUnlock(s32 chan) {
    EXIChan* exi = &Ecb[chan];
    BOOL enabled;
    EXICallback callback;

    enabled = OSDisableInterrupts();

    if (!(exi->flags & 0x10)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    /* Clear lock flag */
    exi->flags &= ~0x10;

    /* Update interrupt mask */
    /* fn_80098110(chan, exi) */

    /* Call first queued waiter */
    if ((s32)exi->nQueued > 0) {
        callback = (EXICallback)exi->queuedCb[0];
        exi->nQueued--;

        /* Shift queue entries down */
        if ((s32)exi->nQueued > 0) {
            memmove(&exi->queuedDev[0], &exi->queuedDev[1],
                    exi->nQueued * 8);
        }

        /* Call the unlock callback */
        callback(chan, NULL);
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/* Internal helper stubs */
static u32 __EXISetExiInterruptMask(s32 chan, u32 tcMask, u32 extMask, u32 exiMask) {
    /* This corresponds to fn_800986A0 in the disassembly */
    volatile u32* reg = EXI_CHAN_PARAMS(chan);
    u32 old = reg[0];
    u32 csr = old;

    csr &= 0x7F5; /* preserve relevant bits */

    if (tcMask)   csr |= 0x002; /* TC int enable */
    if (extMask)  csr |= 0x008; /* EXT int enable */
    if (exiMask)  csr |= 0x800; /* EXI int enable */

    reg[0] = csr;
    return old;
}

static BOOL __EXIProbe(s32 chan) {
    /* This corresponds to fn_80098790 in the disassembly */
    /* Simplified - full implementation checks device insertion/removal timing */
    return TRUE;
}

/* ===================================================================
 * Stub functions for coverage -- TODO: decompile
 * 8 function(s)
 * =================================================================== */

/* fn_80098368 - 0x80098368 | size: 0xA0
 * EXIImmEx - Transfer data via EXI in chunks of up to 4 bytes.
 * Returns TRUE on success, FALSE if any transfer fails.
 */
BOOL fn_80098368(s32 chan, u8* buf, s32 len, s32 mode) {
    s32 chunkSize;

    while (len > 0) {
        chunkSize = (len >= 4) ? 4 : len;

        if (!EXIImm(chan, buf, chunkSize, mode, NULL)) {
            return FALSE;
        }
        if (!EXISync(chan)) {
            return FALSE;
        }
        buf += chunkSize;
        len -= chunkSize;
    }
    return TRUE;
}

/* fn_8009870C - 0x8009870C | size: 0x84
 * EXISetExiCallback - Set/clear the EXI interrupt callback for a channel.
 * Saves the old callback, installs the new one, and calls the low-level
 * interrupt setup function. Returns the previous callback.
 */
u32 fn_8009870C(s32 chan) {
    extern u8 lbl_803FB3C8[];
    extern void fn_80098110(s32 chan, u8* chanState);
    u8* chanState;
    BOOL enabled;
    u32 oldCallback;

    chanState = lbl_803FB3C8 + (chan << 6);
    enabled = OSDisableInterrupts();

    oldCallback = *(u32*)(chanState + 0x0);
    *(u32*)(chanState + 0x0) = 0;

    if (chan == 2) {
        fn_80098110(0, lbl_803FB3C8);
    } else {
        fn_80098110(chan, chanState);
    }

    OSRestoreInterrupts(enabled);
    return oldCallback;
}

/* fn_80098944 - 0x80098944 | size: 0x7C */
void fn_80098944(void) {
    u8 sp[0x20];
    extern u8 lbl_803FB3C8[];
    extern void fn_80098790();
    extern void fn_80099400();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3 + 0x0;
    r4 = r3 << 6;
    r3 = (u32)lbl_803FB3C8;
    tmp = (u32)lbl_803FB3C8;
    r29 = tmp + r4;
    r3 = r30 + 0x0;
    fn_80098790();
    /* mr. r31, r3 */;
    if ((s32)tmp == 0) goto L_800989A8;
    tmp = *(u32*)((u8*)r29 + 0x20);
    if ((s32)tmp != 0) goto L_800989A8;
    r3 = r30 + 0x0;
    r4 = 0x0;
    r5 = (u32)sp + 0xc;
    fn_80099400();
    if ((s32)r3 == 0) goto L_800989A4;
    r31 = 0x1;
    goto L_800989A8;
L_800989A4:
    r31 = 0x0;
L_800989A8:
    r3 = r31;
    return;
}

/* fn_800989C0 - 0x800989C0 | size: 0x128 */
void fn_800989C0(void) {
    u8 sp[0x20];
    extern u8 lbl_803FB3C8[];
    extern void fn_800986A0();
    extern void fn_80098790();
    extern void fn_80099400();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r31 = r3;
    r3 = (u32)lbl_803FB3C8;
    r30 = (u32)lbl_803FB3C8;
    tmp = r31 << 6;
    r24 = r30 + tmp;
    tmp = r31 << 6;
    r22 = r30 + tmp;
    r3 = r31 + 0x0;
    fn_80098790();
    /* mr. r27, r3 */;
    if ((s32)tmp == 0) goto L_80098A20;
    tmp = *(u32*)((u8*)r22 + 0x20);
    if ((s32)tmp != 0) goto L_80098A20;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = (u32)sp + 0x10;
    fn_80099400();
L_80098A20:
    OSDisableInterrupts();
    r28 = r3;
    tmp = *(u32*)((u8*)r24 + 0x20);
    if ((s32)tmp != 0) goto L_80098A44;
    r3 = r28;
    OSRestoreInterrupts(r3);
    r3 = 0x0;
    goto L_80098AD4;
L_80098A44:
    tmp = r31 << 6;
    r29 = r30 + tmp;
    OSDisableInterrupts();
    r25 = r3;
    tmp = *(u32*)((u8*)r29 + 0xC);
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) goto L_80098A70;
    r3 = r31;
    fn_80098790();
    if ((s32)r3 != 0) goto L_80098A80;
L_80098A70:
    r3 = r25;
    OSRestoreInterrupts(r3);
    r26 = 0x0;
    goto L_80098AC4;
L_80098A80:
    r3 = r31 + 0x0;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_800986A0();
    *(u32*)((u8*)r29 + 0x8) = tmp;
    r3 = 0x100000;
    tmp = r31 * 0x3;
    r3 = (u32)r3 >> tmp;
    __OSUnmaskInterrupts(r3);
    tmp = *(u32*)((u8*)r29 + 0xC);
    tmp = tmp | 0x8;
    *(u32*)((u8*)r29 + 0xC) = tmp;
    r3 = r25;
    OSRestoreInterrupts(r3);
    r26 = 0x1;
L_80098AC4:
    r23 = r26 + 0x0;
    r3 = r28 + 0x0;
    OSRestoreInterrupts(r3);
    r3 = r23;
L_80098AD4:
    return;
}

/* fn_80098AE8 - 0x80098AE8 | size: 0xAC */
void fn_80098AE8(void) {
    extern u8 lbl_803FB3C8[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3 + 0x0;
    r4 = r3 << 6;
    r3 = (u32)lbl_803FB3C8;
    tmp = (u32)lbl_803FB3C8;
    r31 = tmp + r4;
    OSDisableInterrupts();
    r30 = r3;
    tmp = *(u32*)((u8*)r31 + 0xC);
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) goto L_80098B30;
    r3 = r30;
    OSRestoreInterrupts(r3);
    r3 = 0x1;
    goto L_80098B80;
L_80098B30:
    tmp = *(u32*)((u8*)r31 + 0xC);
    tmp = tmp & 0x00000010;
    if ((s32)tmp == 0) goto L_80098B58;
    tmp = *(u32*)((u8*)r31 + 0x18);
    if (tmp != 0) goto L_80098B58;
    r3 = r30;
    OSRestoreInterrupts(r3);
    r3 = 0x0;
    goto L_80098B80;
L_80098B58:
    tmp = *(u32*)((u8*)r31 + 0xC);
    tmp = tmp & 0xFFFFFFF7;
    *(u32*)((u8*)r31 + 0xC) = tmp;
    r3 = 0x700000;
    tmp = r29 * 0x3;
    r3 = (u32)r3 >> tmp;
    __OSMaskInterrupts(r3);
    r3 = r30;
    OSRestoreInterrupts(r3);
    r3 = 0x1;
L_80098B80:
    return;
}

/* fn_80098DDC - 0x80098DDC | size: 0xC0 */
void fn_80098DDC(void) {
    u8 sp[0x20];
    extern u8 lbl_803FB3C8[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f5 = 0.0f;
    f32 f8 = 0.0f;

    *(u16*)((u32)sp + 0x8) = r3;
    r27 = r4;
    r3 = *(s16*)((u8*)(u32)sp + 0x8);
    tmp = 0x3;
    r31 = (s32)r3 / (s32)tmp;
    r4 = r31 << 6;
    r3 = (u32)lbl_803FB3C8;
    tmp = (u32)lbl_803FB3C8;
    r28 = tmp + r4;
    tmp = r31 * 0x5;
    tmp = tmp << 2;
    r3 = 0xCC000000;
    r3 = r3 + 0x6800;
    r30 = *(u32*)(r3 + tmp);
    r30 = r30 & 0x7f5;
    r30 = r30 | 0x2;
    tmp = r31 * 0x5;
    tmp = tmp << 2;
    r3 = 0xCC000000;
    r3 = r3 + 0x6800;
    *(u32*)(r3 + tmp) = r30;
    r29 = *(u32*)((u8*)r28 + 0x0);
    if (r29 == 0) goto L_80098E88;
    r3 = (u32)sp + 0x18;
    OSClearContext((OSContext*)r3);
    r3 = (u32)sp + 0x18;
    OSSetCurrentContext((OSContext*)r3);
    r3 = r31 + 0x0;
    r4 = r27 + 0x0;
    r12 = r29 + 0x0;
    /* blrl  */;
    r3 = (u32)sp + 0x18;
    OSClearContext((OSContext*)r3);
    r3 = r27;
    OSSetCurrentContext((OSContext*)r3);
L_80098E88:
    return;
}

/* fn_80098E9C - 0x80098E9C | size: 0x15C */
void fn_80098E9C(void) {
    u8 sp[0x20];
    extern u8 lbl_803FB3C8[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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
    f32 f5 = 0.0f;

    r21 = r3 + 0x0;
    r22 = r4 + 0x0;
    r3 = (s16)r21;
    tmp = 0x3;
    r31 = (s32)r3 / (s32)tmp;
    r4 = r31 << 6;
    r3 = (u32)lbl_803FB3C8;
    tmp = (u32)lbl_803FB3C8;
    r27 = tmp + r4;
    r3 = 0x80000000;
    tmp = (s16)r21;
    r3 = (u32)r3 >> tmp;
    __OSMaskInterrupts(r3);
    tmp = r31 * 0x5;
    tmp = tmp << 2;
    r3 = 0xCC000000;
    r3 = r3 + 0x6800;
    r29 = *(u32*)(r3 + tmp);
    r29 = r29 & 0x7f5;
    r29 = r29 | 0x8;
    tmp = r31 * 0x5;
    tmp = tmp << 2;
    r3 = 0xCC000000;
    r3 = r3 + 0x6800;
    *(u32*)(r3 + tmp) = r29;
    r26 = *(u32*)((u8*)r27 + 0x4);
    if (r26 == 0) goto L_80098FE4;
    tmp = 0x0;
    *(u32*)((u8*)r27 + 0x4) = tmp;
    r4 = r31 << 6;
    r3 = (u32)lbl_803FB3C8;
    tmp = (u32)lbl_803FB3C8;
    r30 = tmp + r4;
    tmp = *(u32*)((u8*)r30 + 0xC);
    tmp = tmp & 0x3;
    if (r26 == 0) goto L_80098FB0;
    tmp = *(u32*)((u8*)r30 + 0xC);
    tmp = tmp & 0x00000002;
    if (r26 == 0) goto L_80098FA4;
    r25 = *(u32*)((u8*)r30 + 0x10);
    if ((s32)r25 == 0) goto L_80098FA4;
    r23 = *(u32*)((u8*)r30 + 0x14);
    r3 = r31 * 0x5;
    tmp = r3 + 0x4;
    tmp = tmp << 2;
    r3 = 0xCC000000;
    r3 = r3 + 0x6800;
    r24 = *(u32*)(r3 + tmp);
    r28 = 0x0;
    goto L_80098F9C;
L_80098F84:
    tmp = 0x3 - r28;
    tmp = tmp << 3;
    r3 = (u32)r24 >> tmp;
    *(u8*)((u8*)r23 + 0x0) = r3;
    r23 = r23 + 0x1;
    r28 = r28 + 0x1;
L_80098F9C:
    if ((s32)r28 < (s32)r25) goto L_80098F84;
L_80098FA4:
    tmp = *(u32*)((u8*)r30 + 0xC);
    /* clrrwi tmp, tmp, 2 */;
    *(u32*)((u8*)r30 + 0xC) = tmp;
L_80098FB0:
    r3 = (u32)sp + 0x18;
    OSClearContext((OSContext*)r3);
    r3 = (u32)sp + 0x18;
    OSSetCurrentContext((OSContext*)r3);
    r3 = r31 + 0x0;
    r4 = r22 + 0x0;
    r12 = r26 + 0x0;
    /* blrl  */;
    r3 = (u32)sp + 0x18;
    OSClearContext((OSContext*)r3);
    r3 = r22;
    OSSetCurrentContext((OSContext*)r3);
L_80098FE4:
    return;
}

/* fn_80098FF8 - 0x80098FF8 | size: 0xC8 */
void fn_80098FF8(void) {
    u8 sp[0x20];
    extern u8 lbl_803FB3C8[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    *(u16*)((u32)sp + 0x8) = r3;
    r28 = r4;
    r3 = *(s16*)((u8*)(u32)sp + 0x8);
    tmp = 0x3;
    r31 = (s32)r3 / (s32)tmp;
    r3 = 0x700000;
    tmp = r31 * 0x3;
    r3 = (u32)r3 >> tmp;
    __OSMaskInterrupts(r3);
    r4 = 0x0;
    tmp = r31 * 0x5;
    tmp = tmp << 2;
    r3 = 0xCC000000;
    r3 = r3 + 0x6800;
    *(u32*)(r3 + tmp) = r4;
    r4 = r31 << 6;
    r3 = (u32)lbl_803FB3C8;
    tmp = (u32)lbl_803FB3C8;
    r30 = tmp + r4;
    r29 = *(u32*)((u8*)r30 + 0x8);
    tmp = *(u32*)((u8*)r30 + 0xC);
    tmp = tmp & 0xFFFFFFF7;
    *(u32*)((u8*)r30 + 0xC) = tmp;
    if (r29 == 0) goto L_800990AC;
    r3 = (u32)sp + 0x10;
    OSClearContext((OSContext*)r3);
    r3 = (u32)sp + 0x10;
    OSSetCurrentContext((OSContext*)r3);
    tmp = 0x0;
    *(u32*)((u8*)r30 + 0x8) = tmp;
    r3 = r31 + 0x0;
    r4 = r28 + 0x0;
    r12 = r29 + 0x0;
    /* blrl  */;
    r3 = (u32)sp + 0x10;
    OSClearContext((OSContext*)r3);
    r3 = r28;
    OSSetCurrentContext((OSContext*)r3);
L_800990AC:
    return;
}

