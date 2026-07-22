/**
 * @file sdk_range_800A7BD4.c
 * @brief Dolphin DVD disk-state query, 0x800A7BD4 - 0x800A7CCC.
 *
 * The source is byte-exact in isolation, but its compiler-generated jump table
 * starts at a four-byte-aligned retail address. Keep the function candidate-side
 * until its original translation-unit data alignment can be restored naturally.
 */
#include "dolphin/types.h"
#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"

extern s32 FatalErrorFlag_8047A800;
extern s32 PausingFlag_8047A7F8;
extern DVDCommandBlock* executing_8047A7E8;
extern DVDCommandBlock DummyCommandBlock_803FC3A0;
extern u32 ResumeFromHere_8047A810;
extern volatile u32 __DIRegs[16] : 0xCC006000;

BOOL DVDCheckDisk(void) {
    BOOL enabled;
    s32 result;
    s32 state;
    u32 cover;

    enabled = OSDisableInterrupts();

    if (FatalErrorFlag_8047A800) {
        state = -1;
    } else if (PausingFlag_8047A7F8) {
        state = 8;
    } else if (executing_8047A7E8 == NULL) {
        state = 0;
    } else if (executing_8047A7E8 == &DummyCommandBlock_803FC3A0) {
        state = 0;
    } else {
        state = executing_8047A7E8->state;
    }

    switch (state) {
    case 1:
    case 2:
    case 9:
    case 10:
        result = TRUE;
        break;
    case -1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 11:
        result = FALSE;
        break;
    case 0:
    case 8:
        cover = __DIRegs[1];
        if (((cover >> 2) & 1) || (cover & 1)) {
            result = FALSE;
        } else if (ResumeFromHere_8047A810 != 0) {
            result = FALSE;
        } else {
            result = TRUE;
        }
        break;
    }

    OSRestoreInterrupts(enabled);
    return result;
}
