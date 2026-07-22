#include "dolphin/types.h"
#include "hsd/hsd_aobj.h"

extern s32 lbl_8047B390;
extern s32 lbl_8047B38C;
extern HSD_SList* lbl_8047B388;

void HSD_AObjReqAnim(HSD_AObj* aobj, f32 frame)
{
    u32 flags;

    if (aobj == NULL) {
        return;
    }
    aobj->curr_frame = frame;
    flags = aobj->flags & ~AOBJ_NO_ANIM;
    aobj->flags = flags | AOBJ_FIRST_PLAY;
    HSD_FObjReqAnimAll(aobj->fobj, frame);
}

void HSD_AObjInvokeCallBacks(void)
{
    HSD_SList* list;

    if (lbl_8047B390 != 0 && lbl_8047B38C == 0) {
        list = lbl_8047B388;
        while (list != NULL) {
            void (*func)(void) = (void (*)(void))list->data;
            (*func)();
            list = list->next;
        }
    }
}
