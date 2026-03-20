#ifndef DOLPHIN_OS_OSCONTEXT_H
#define DOLPHIN_OS_OSCONTEXT_H

#include "dolphin/types.h"

/* OS_CONTEXT_* state flags at offset 0x1A2 */
#define OS_CONTEXT_STATE_FPSAVED 0x0001
#define OS_CONTEXT_STATE_EXC     0x0002

typedef struct OSContext {
    /* 0x000 */ u32 gpr[32];       /* General-purpose registers */
    /* 0x080 */ u32 cr;            /* Condition register */
    /* 0x084 */ u32 lr;            /* Link register */
    /* 0x088 */ u32 ctr;           /* Count register */
    /* 0x08C */ u32 xer;           /* XER register */
    /* 0x090 */ f64 fpr[32];       /* Floating-point registers */
    /* 0x190 */ u64 fpscr_pad;     /* FPSCR as double */
    /* 0x198 */ u32 srr0;          /* SRR0 */
    /* 0x19C */ u32 srr1;          /* SRR1 */
    /* 0x1A0 */ u16 mode;          /* context mode */
    /* 0x1A2 */ u16 state;         /* OS_CONTEXT_STATE_* */
    /* 0x1A4 */ u32 gqr[8];       /* GQR0-7 */
    /* 0x1C4 */ u32 _padding;
    /* 0x1C8 */ f64 psf[32];       /* Paired singles (second half) */
} OSContext;

void OSSetCurrentContext(OSContext* context);
void OSGetCurrentContext(OSContext* context);
void OSLoadContext(OSContext* context);
void OSClearContext(OSContext* context);
void OSSaveFPUContext(OSContext* context);

void OSSwitchFPUContext(u8 exception, OSContext* context);

void __OSContextInit(void);
void __OSLoadFPUContext(u8 unused, OSContext* context);
void __OSSaveFPUContext(u8 unused, OSContext* context);

u32 OSSaveContext(OSContext* context);
void OSDumpContext(OSContext* context);

#endif /* DOLPHIN_OS_OSCONTEXT_H */
