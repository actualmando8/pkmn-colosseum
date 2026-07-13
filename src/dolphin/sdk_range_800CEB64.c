/**
 * @file sdk_range_800CEB64.c
 * @brief dolphin-sdk code, 0x800CEB64 - 0x800CF708 (13 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/os/OSInterrupt.h"

typedef void (*DBGInterruptHandler)(__OSInterrupt interrupt, OSContext* context);

extern DBGInterruptHandler lbl_8047AA2C;
extern void (*lbl_8047AA28)(s32);
extern u8 lbl_8047AA3C;

void DBGHandler(s32 interrupt, OSContext* context) {
    *(volatile u32*)0xCC003000 = 0x1000;
    if (lbl_8047AA2C != NULL) {
        lbl_8047AA2C((__OSInterrupt)interrupt, context);
    }
}

void MWCallback(void) {
    lbl_8047AA3C = 1;
    if (lbl_8047AA28 != NULL) {
        lbl_8047AA28(0);
    }
}
