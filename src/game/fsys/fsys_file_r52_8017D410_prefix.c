/* Score instrumentation only; not evidence of a retail TU boundary. */
#include "src/game/fsys/fsys_file_candidate_8017D410.c"

FSYSSlot* fn_8017D410(u32 fileHandle, u32 mode)
{
    u32 i;
    FSYSSlot* slot;

    slot = (FSYSSlot*)lbl_8047B1B4;
    for (i = 0; i < lbl_80453FEC.maxSlots; i++, slot++) {
        if (slot->status == 0 || slot->fileHandle != fileHandle) {
            continue;
        }

        if (slot->status != 1000) {
            if (mode == 2 || mode == 7) {
                return NULL;
            }
            if (slot->loadMode == 3) {
                return slot;
            }
            slot->reloadFlag = 1;
            return slot;
        }

        switch (mode) {
        case 0:
            if (slot->loadMode != 2 && slot->loadMode != 7) {
                slot->refCount++;
            }
            break;
        case 2:
        case 7:
            if (slot->refCount == 0) {
                slot->refCount++;
            }
            break;
        }
        return slot;
    }

    slot = (FSYSSlot*)lbl_8047B1B4;
    for (i = 0; i < lbl_80453FEC.maxSlots; i++, slot++) {
        if (slot->status == 0) {
            slot->refCount = 0;
            slot->refCount++;
            return slot;
        }
    }
    return NULL;
}
