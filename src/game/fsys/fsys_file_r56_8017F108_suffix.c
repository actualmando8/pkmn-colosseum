/* Score instrumentation only; not evidence of a retail TU boundary. */
#include "src/game/fsys/fsys_file_candidate_8017EB6C.c"

void fn_8017F108(s32 result)
{
    u32 enabled;
    FSYSSlot* slot;

    enabled = OSDisableInterrupts();
    slot = lbl_80453FEC.activeSlot;

    if (result == -1) {
        slot->archiveHandle = -1;
    } else {
        switch (slot->status) {
        case 1:
            slot->status = 2;
            break;
        case 3:
            slot->status = 4;
            break;
        case 101:
            slot->status = 150;
            break;
        case 200:
            slot->status = 201;
            break;
        case 303:
            slot->status = 304;
            break;
        case 400:
            slot->status = 401;
            break;
        case 2:
        case 4:
        case 100:
        case 201:
        case 301:
            break;
        default:
            slot->status = 1000;
            slot->archiveHandle = 1;
            break;
        }
    }

    if (slot->tocBuffer != NULL) {
        fn_80167E64((u32)slot->tocBuffer);
        slot->tocBuffer = NULL;
    }
    OSRestoreInterrupts(enabled);
}
