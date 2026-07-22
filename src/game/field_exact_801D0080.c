#include "dolphin/types.h"

typedef struct MemcardTaskState {
    s32 task_kind;
    s32 error_code;
    s32 task_result;
    s32 state;
    s32 resume_state;
    s32 card_channel;
    s32 sector_size;
    s32 memory_size;
    s32 field_20;
    s32 retry_count;
    s32 card_result;
    s32 field_2c;
    u32 serial_hi;
    s32 random_delay;
    u8 field_38[4];
    u8 callback_finished;
} MemcardTaskState;

extern MemcardTaskState* lbl_8047B3D4;

void fn_801D0080(void)
{
    lbl_8047B3D4->callback_finished = 1;
}
