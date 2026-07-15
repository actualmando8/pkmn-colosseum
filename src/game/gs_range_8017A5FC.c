/**
 * @file gs_range_8017A5FC.c
 * @brief gs-engine code, 0x8017A5FC - 0x8017B07C (9 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

#include "game/fsys/fsys.h"

/* Address: 0x8017A5FC | size: 0x28 */
extern FSYSManager lbl_80453FEC;
#pragma optimization_level 0
void fn_8017A5FC(void)
{
    FSYSSlot* slot;

    slot = lbl_80453FEC.activeSlot;
    slot->status = 0x12f;
}
#pragma optimization_level reset

u32 fn_8017AC30(void)
{
    return lbl_80453FEC.field_28;
}

extern FSYSSlot* fn_8017D410(u32 fileHandle, u32 mode);
extern u8 fn_8017E30C(FSYSSlot* slot);

#pragma optimization_level 0
s32 fn_8017AF6C(u32 fileHandle, u32 requestID)
{
    FSYSSlot* slot;

    slot = fn_8017D410(fileHandle, 3);
    if (slot->fileHandle == fileHandle) {
        slot->requestID = requestID;
        slot->callbackA = 0;
        slot->callbackB = 0;
        slot->callbackC = 0;
        if (fn_8017E30C(slot)) {
            return 1;
        }
    } else {
        return 0;
    }
    return 0;
}
#pragma optimization_level reset

extern void fn_8017E1D8(FSYSSlot* slot, u32 fileHandle, u32 callbackA,
                         u32 callbackB, u32 callbackC);

#pragma optimization_level 0
s32 fn_8017B000(u32 fileHandle, u32 requestID, u32 callbackA, u32 callbackB,
                u32 callbackC)
{
    FSYSSlot* slot;

    slot = fn_8017D410(fileHandle, 3);
    if (slot != 0) {
        slot->requestID = requestID;
        fn_8017E1D8(slot, fileHandle, callbackA, callbackB, callbackC);
        return 1;
    }
    return 0;
}
#pragma optimization_level reset
