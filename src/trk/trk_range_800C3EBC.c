/**
 * @file trk_range_800C3EBC.c
 * @brief trk code, 0x800C3EBC - 0x800C41A4 (4 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct CircleBuffer {
    u8* readPtr;
    u8* writePtr;
    u8* buffer;
    u32 size;
    u32 used;
    u32 free;
    u32 state;
} CircleBuffer;

/* ddh_cc_initialize - 0x800C3EBC | size: 0x88 | scope global */
s32 ddh_cc_initialize(u8** comm, void (*callback)(s32)) {
    extern const char lbl_8026FD4C[];
    extern const char lbl_8026FD60[];
    extern u8 lbl_803FED78[];
    extern CircleBuffer lbl_803FF578;
    extern void MWTRACE(s32 level, const char* format, ...);
    extern void fn_800CE79C(u8** comm, void (*callback)(s32));
    extern void CircleBufferInitialize(CircleBuffer* circle, u8* buffer, u32 size);

    MWTRACE(1, lbl_8026FD4C);
    fn_800CE79C(comm, callback);
    MWTRACE(1, lbl_8026FD60);
    CircleBufferInitialize(&lbl_803FF578, lbl_803FED78, 0x800);
    return 0;
}

/* CircleBufferInitialize - 0x800C4154 | size: 0x50 | scope global */
void CircleBufferInitialize(CircleBuffer* circle, u8* buffer, u32 size) {
    extern void fn_800C459C(u32* state);

    circle->buffer = buffer;
    circle->size = size;
    circle->readPtr = circle->buffer;
    circle->writePtr = circle->buffer;
    circle->used = 0;
    circle->free = circle->size;
    fn_800C459C(&circle->state);
}
