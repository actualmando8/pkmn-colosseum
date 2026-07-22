#include "dolphin/exi/EXI.h"
#include "dolphin/os/OSContext.h"
#include "src/dolphin/card_dsp_private.h"

extern EXICallback fn_8009870C(s32 chan, EXICallback callback);

void __DSP_insert_task(DSPTaskInfo* task)
{
    DSPTaskInfo* current;

    if (lbl_8047A968 == NULL) {
        lbl_8047A96C = task;
        lbl_8047A964 = task;
        lbl_8047A968 = task;
        task->prev = NULL;
        task->next = NULL;
        return;
    }

    current = lbl_8047A968;
    while (current != NULL) {
        if (task->priority < current->priority) {
            task->prev = current->prev;
            current->prev = task;
            task->next = current;
            if (task->prev == NULL) {
                lbl_8047A968 = task;
            } else {
                task->prev->next = task;
            }
            break;
        }
        current = current->next;
    }

    if (current == NULL) {
        lbl_8047A964->next = task;
        task->next = NULL;
        task->prev = lbl_8047A964;
        lbl_8047A964 = task;
    }
}

void __DSP_remove_task(DSPTaskInfo* task)
{
    task->flags = 0;
    task->state = 3;

    if (lbl_8047A968 == task) {
        if (task->next != NULL) {
            lbl_8047A968 = task->next;
            task->next->prev = NULL;
        } else {
            lbl_8047A96C = NULL;
            lbl_8047A964 = NULL;
            lbl_8047A968 = NULL;
        }
        return;
    }

    if (lbl_8047A964 == task) {
        lbl_8047A964 = task->prev;
        task->prev->next = NULL;
        lbl_8047A96C = lbl_8047A968;
        return;
    }

    lbl_8047A96C = task->next;
    task->prev->next = task->next;
    task->next->prev = task->prev;
}

void __CARDDefaultApiCallback(s32 chan, s32 result)
{
}

void __CARDExtHandler(s32 chan, OSContext* context)
{
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;

    if (card->attached != 0) {
        card->attached = 0;
        fn_8009870C(chan, NULL);
        OSCancelAlarm(&card->alarm);
        callback = card->callback_CC;
        if (callback != NULL) {
            card->callback_CC = NULL;
            callback(chan, -3);
        }
        if (card->result != -1) {
            card->result = -3;
        }
        callback = card->extCallback;
        if (callback != NULL && card->field_24 >= 7) {
            card->extCallback = NULL;
            callback(chan, -3);
        }
    }
}
